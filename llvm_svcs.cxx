/* abstract away LLVM details from the rest of LLL

	anything starting with "llvm_svcs" is a public API
	anything else is internal use
*/

/* c++ includes */
#include <map>
#include <string>
#include <vector>
using namespace std;

/* llvm includes */
/* note that at least in 4.0.0 and onward, much gets moved to llvm/MC/MCParser/ */
#include <llvm/MC/MCAsmBackend.h>
#include <llvm/MC/MCAsmInfo.h>
#include <llvm/MC/MCContext.h>
#include <llvm/MC/MCDisassembler.h>
#include <llvm/MC/MCCodeEmitter.h>
#include <llvm/MC/MCInstPrinter.h>
#include <llvm/MC/MCInstrInfo.h>
#include <llvm/MC/MCObjectFileInfo.h>
#include <llvm/MC/MCRegisterInfo.h>
#include <llvm/MC/MCSectionMachO.h>
#include <llvm/MC/MCStreamer.h>
#include <llvm/MC/MCSubtargetInfo.h>
#include <llvm/MC/MCTargetAsmParser.h>
#include <llvm/MC/MCTargetOptionsCommandFlags.h>
#include <llvm/MC/MCParser/AsmLexer.h>

#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Compression.h>
#include <llvm/Support/FileUtilities.h>
#include <llvm/Support/FormattedStream.h>
#include <llvm/Support/Host.h> // for getDefaultTargetTriple();
#include <llvm/Support/ManagedStatic.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/PrettyStackTrace.h>
#include <llvm/Support/Signals.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/ToolOutputFile.h>

#define DIALECT_ATT 0
#define DIALECT_INTEL 1

/* autils */
extern "C" {
#include <autils/output.h>
}

/* local includes */
#include "llvm_svcs.h"

/*****************************************************************************/
/* MISCELLANY */
/*****************************************************************************/

void
llvm_svcs_init(void)
{
	llvm::InitializeAllTargetInfos();
	llvm::InitializeAllTargetMCs();
	llvm::InitializeAllAsmParsers();
	llvm::InitializeAllDisassemblers();
}

/* references:
	http://clang.llvm.org/docs/CrossCompilation.html
	<llvm_source>/include/llvm/ADT/Triple.h
	https://wiki.debian.org/Multiarch/Tuples
	https://www.linux-mips.org/wiki/MIPS_ABI_History
*/
void
llvm_svcs_triplet_decompose(const char *triplet, string &arch, string &subarch,
	string &vendor, string &os, string &environ, string &objFormat)
{
	//spec = llvm::sys::getDefaultTargetTriple();
	//std::string machSpec = "x86_64-apple-darwin14.5.0";
	//std::string machSpec = "x86_64-apple-darwin";
	//std::string machSpec = "x86_64-thumb-linux-gnu";
	//std::string machSpec = "x86_64-unknown-linux-gnu";
	Triple trip(triplet);

	/* FIRST component is <arch><subarch>
		eg: x86,arm,thumb,mips,... */
	arch = trip.getArchName();

	/* sub architecture
		eg: v5,v6m,v7a,v7m,... */
	switch(trip.getSubArch()) {
		case llvm::Triple::NoSubArch: subarch=""; break;
   		case llvm::Triple::ARMSubArch_v8_2a: subarch="v8_2a"; break;
		case llvm::Triple::ARMSubArch_v8_1a: subarch="v8_1a"; break;
		case llvm::Triple::ARMSubArch_v8: subarch="v8"; break;
		/* not here in 3.8.1, here in 3.9.0 */
		//case llvm::Triple::ARMSubArch_v8m_baseline: subarch="v8m"; break;
		//case llvm::Triple::ARMSubArch_v8m_mainline: subarch="v8m"; break; // ??
		case llvm::Triple::ARMSubArch_v7: subarch="v7"; break;
		case llvm::Triple::ARMSubArch_v7em: subarch="v7em"; break;
		case llvm::Triple::ARMSubArch_v7m: subarch="v7m"; break;
		case llvm::Triple::ARMSubArch_v7s: subarch="v7s"; break;
		case llvm::Triple::ARMSubArch_v7k: subarch="v7k"; break;
		case llvm::Triple::ARMSubArch_v6: subarch="v6"; break;
		case llvm::Triple::ARMSubArch_v6m: subarch="v6m"; break;
		case llvm::Triple::ARMSubArch_v6k: subarch="v6k"; break;
		case llvm::Triple::ARMSubArch_v6t2: subarch="v6t2"; break;
		case llvm::Triple::ARMSubArch_v5: subarch="v5"; break;
		case llvm::Triple::ARMSubArch_v5te: subarch="v5te"; break;
		case llvm::Triple::ARMSubArch_v4t: subarch="v4t"; break;
		case llvm::Triple::KalimbaSubArch_v3: subarch="v3"; break;
		case llvm::Triple::KalimbaSubArch_v4: subarch="v4"; break;
		case llvm::Triple::KalimbaSubArch_v5: subarch="v5"; break;
	}

	/* SECOND component is <vendor>
		eg: pc,apple,nvidia,ibm,... */
	vendor = trip.getVendorName();

	/* THIRD component is <sys> or <os>
		eg: none,linux,win32,darwin,cuda,... */
	os = trip.getOSName();

	/* FOURTH component is <environment> or <abi>
		eg: eabi,gnu,android.macho,elf,... */
	environ = trip.getEnvironmentName();

	/* this is not part of the triplet, just trivia about what object format
		will be used to contain the code */
	Triple::ObjectFormatType oft = trip.getObjectFormat();

	switch(oft) {
		case Triple::COFF: objFormat = "coff"; break;
		case Triple::ELF: objFormat = "elf"; break;
		case Triple::MachO: objFormat = "MachO"; break;
		case Triple::UnknownObjectFormat: objFormat = "unknown"; break;
	}
}

/* map LLVM services code model to LLVM code model
	(so API users don't have to include LLVM headers) */
CodeModel::Model
map_code_model(int codeModel)
{
	/* SEE: include/llvm-c/TargetMachine.h */
	switch(codeModel) {
		case LLVM_SVCS_CM_DEFAULT: return CodeModel::JITDefault;
		case LLVM_SVCS_CM_SMALL: return CodeModel::Small;
		case LLVM_SVCS_CM_KERNEL: return CodeModel::Kernel;
		case LLVM_SVCS_CM_MEDIUM: return CodeModel::Medium;
		case LLVM_SVCS_CM_LARGE: return CodeModel::Large;
		default: return CodeModel::JITDefault;
	}
}

/* map LLVM services reloc mode to LLVM reloc mode
	(so API users don't have to include LLVM headers) */
Reloc::Model
map_reloc_mode(int relocMode)
{
	/* SEE: NOT the values in include/llvm-c/TargetMachine.h
		but llvm/Support/CodeGen.h */
	switch(relocMode) {
		case LLVM_SVCS_RM_STATIC: return Reloc::Static;
		case LLVM_SVCS_RM_PIC: return Reloc::PIC_;
		case LLVM_SVCS_RM_DYNAMIC_NO_PIC: return Reloc::DynamicNoPIC;
		default: return Reloc::Static;
	}
}

/*****************************************************************************/
/* ENDIAN HELPERS */
/*****************************************************************************/

uint32_t bswap32(uint32_t x)
{
	return ((x&0xFF)<<24) |
		((x&0xFF00)<<8) |
		((x&0xFF0000)>>8) |
		((x&0xFF000000)>>24);
}

uint64_t bswap64(uint64_t x)
{
	return ((x&0xFF)<<56) |
		((x&0xFF00)<<40) |
		((x&0xFF0000)<<24) |
		((x&0xFF000000)<<8) |
		((x&0xFF00000000)>>8) |
		((x&0xFF0000000000)>>24) |
		((x&0xFF000000000000)>>40) |
		((x&0xFF00000000000000)>>56);

}

uint32_t fetch32(uint8_t *data, bool swap=false)
{
	uint32_t tmp = *(uint32_t *)data;
	if(swap) tmp = __builtin_bswap32(tmp);
	return tmp;
}

uint64_t fetch64(uint8_t *data, bool swap=false)
{
	uint64_t tmp = *(uint64_t *)data;
	if(swap) tmp = __builtin_bswap64(tmp);
	return tmp;
}

void set32(uint8_t *addr, uint32_t val, bool swap=false)
{
	if(swap) val = __builtin_bswap32(val);
	*(uint32_t *)addr = val;
}

void set64(uint8_t *addr, uint64_t val, bool swap=false)
{
	if(swap) val = __builtin_bswap64(val);
	*(uint64_t *)addr = val;
}

/*****************************************************************************/
/* ASSEMBLE RELATED FUNCTIONS */
/*****************************************************************************/

int
obj_output_to_bytes(uint8_t *data, string &result)
{
	int rc = -1;

	int codeOffset=0, codeSize = 0;

	/* it's assumed that in all ELF files, the .text section is third, usually
		the layout is NULL, .strtab, .text, ... */

	/* Mach-O */
	if(*(uint32_t *)data == 0xFEEDFACF) {
		unsigned int idx = 0;
		idx += 0x20; /* skip mach_header_64 to first command */
		idx += 0x48; /* advance into segment_command_64 to first section */
		idx += 0x28; /* advance into section_64 to size */
		uint64_t scn_size = *(uint64_t *)(data + idx);
		idx += 0x8; /* advance into section_64 to offset */
		uint64_t scn_offset = *(uint64_t *)(data + idx);
		codeOffset = scn_offset;
		codeSize = scn_size;
	}
	/* 32/64 ELF */
	else if(0==memcmp(data, "\x7F" "ELF\x01\x01\x01\x00", 8) || /* little endian */
	  0==memcmp(data, "\x7F" "ELF\x01\x02\x01\x00", 8)) { /* big endian */

		bool be = data[5] == 0x02;

		/* possibilities:
			- four sections: NULL, .strtab, .text, .symtab
			- five sections: NULL, .strtab, .text, .rel.text, .symtab */
		uint32_t e_shoff = fetch32(data + 0x20, be);
		uint16_t e_shnum = *(uint16_t *)(data + 0x30);
		uint32_t txt_offs = fetch32(data + e_shoff + 2*0x28 + 0x10, be); /* second shdr */
		uint32_t txt_size = fetch32(data + e_shoff + 2*0x28 + 0x14, be); /* second shdr */
		codeOffset = txt_offs;
		codeSize = txt_size;

		if(e_shnum == 5) {
			/* we have relocations, uh oh */
			uint32_t rel_offs = fetch32(data + e_shoff + 3*0x28 + 0x10, be);
			uint32_t rel_size = fetch32(data + e_shoff + 3*0x28 + 0x14, be);
			uint32_t sym_offs = fetch32(data + e_shoff + 4*0x28 + 0x10, be);
			uint32_t sym_size = fetch32(data + e_shoff + 4*0x28 + 0x14, be);
			/* parse relocations */
			for(uint32_t i=0; i<rel_size; i+=8) {
				uint32_t d_val = fetch32(data + rel_offs + i, be);
				uint32_t d_ptr = fetch32(data + rel_offs + i + 4, be);
				/* if R_ARM_CALL */
				if((d_ptr & 0xFF) == 0x1C) {
					uint8_t sym_idx = (d_ptr & 0xFF00) >> 8;
					if(sym_idx >= (sym_size / 16)) continue;
					uint32_t st_value = fetch32(data + sym_offs + 16*sym_idx + 4, be);
					/* at d_val we should find a bl/blx (0xEB/0xFA) */
					uint32_t instr = fetch32(data + txt_offs + d_val, be);
					/* remember pc bias of +8 and this is 4-byte instr count */
					int32_t displ = (st_value - (d_val + 8)) / 4;
					/* modify the instr operand: 3 byte displacement */
					instr = (instr & 0xFF000000) | (displ & 0xFFFFFF);
					set32(data + txt_offs + d_val, instr, be);
				}
			}
		}
	}
	/* 64-bit ELF */
	else if(0==memcmp(data, "\x7F" "ELF\x02\x01\x01\x00", 8)) {
		/* assume text is third section (eg: NULL, .strtab, .text) */
		uint64_t e_shoff = *(uint64_t *)(data + 0x28);
		uint64_t sh_offset = *(uint64_t *)(data + e_shoff + 2*0x40 + 0x18); /* second shdr */
		uint64_t sh_size = *(uint64_t *)(data + e_shoff + 2*0x40 + 0x20); /* second shdr */
		codeOffset = sh_offset;
		codeSize = sh_size;
	}
	/* 64-bit ELF (big-end) */
	else if(0==memcmp(data, "\x7F" "ELF\x02\x02\x01\x00", 8)) {
		/* assume text is third section (eg: NULL, .strtab, .text) */
		uint64_t e_shoff = __builtin_bswap64(*(uint64_t *)(data + 0x28));
		uint64_t sh_offset = __builtin_bswap64(*(uint64_t *)(data + e_shoff + 2*0x40 + 0x18)); /* second shdr */
		uint64_t sh_size = __builtin_bswap64(*(uint64_t *)(data + e_shoff + 2*0x40 + 0x20)); /* second shdr */
		codeOffset = sh_offset;
		codeSize = sh_size;
	}
	else {
		printf("ERROR: couldn't identify type of output file\n");
		goto cleanup;
	}

	result.assign(reinterpret_cast<char const *>(data+codeOffset), codeSize);

	rc = 0;
	cleanup:
	return rc;
}

/* source manager diagnostics handler
	(instead of printing to stderr) */
/* we set LLVM's callback to this thunk which then calls the user
	requested callback, hiding LLVM details */
static
void
diag_cb(const SMDiagnostic &diag, void *param)
{
	if(!param) return;

	#ifdef __GNUC__
	__extension__
	#endif
	llvm_svcs_assemble_cb_type pfunc = (llvm_svcs_assemble_cb_type)param;

	int k;
	switch(diag.getKind()) {
		case SourceMgr::DK_Note: k=LLVM_SVCS_CB_NOTE; break;
		case SourceMgr::DK_Warning: k=LLVM_SVCS_CB_WARNING; break;
		case SourceMgr::DK_Error: k=LLVM_SVCS_CB_ERROR; break;
	}

	// call back back to llvm_svcs user
	string fileName = diag.getFilename().str();
	string message = diag.getMessage().str();
	pfunc(k, fileName.c_str(), diag.getLineNo(), message.c_str());
}

int
invoke_llvm_parsers(const Target *TheTarget, SourceMgr *SrcMgr, MCContext &context,
	MCStreamer &Str, MCAsmInfo &MAI, MCSubtargetInfo &STI, MCInstrInfo &MCII,
	MCTargetOptions &MCOptions, int dialect)
{
	int rc = -1;

	/* create a vanilla (non-target) AsmParser (lib/MC/MCParser/AsmParser.cpp) */
	std::unique_ptr<MCAsmParser> Parser(createMCAsmParser(*SrcMgr, context, Str, MAI));

	/* set the dialect (otherwise it defaults to assemblerInfo's dialect) */
	switch(dialect) {
		case LLVM_SVCS_DIALECT_UNSPEC:
			break;
		case LLVM_SVCS_DIALECT_ATT:
			Parser->setAssemblerDialect(DIALECT_ATT);
			break;
		case LLVM_SVCS_DIALECT_INTEL:
			Parser->setAssemblerDialect(DIALECT_INTEL);
			break;
	}

	/* TARGET asm parser */
  	std::unique_ptr<MCTargetAsmParser> TAP(TheTarget->createMCAsmParser(STI, *Parser, MCII, MCOptions));

  	if (!TAP) {
		printf("ERROR: createMCAsmParser() (does target support assembly parsing?)\n");
		goto cleanup;
	}

	Parser->setTargetParser(*TAP);

  	// AsmParser::Run in lib/MC/MCParser/AsmParser.cpp
  	/* first param is NoInitialTextSection
	   by supplying false -> YES initial text section and obviate ".text" in asm source */
  	rc = Parser->Run(false);

	cleanup:
  	return rc;
}

int
llvm_svcs_assemble(
	/* in parameters */
	const char *srcText, 		/* eg: "mov r0, r0" */
	int dialect, 				/* eg: LLVM_SVCS_DIALECT_ATT */
	const char *triplet, 		/* eg: x86_64-thumb-linux-gnu */
	int codeModel,				/* LLVM_SVCS_CM_JIT, LLVM_SVCS_CM_SMALL, etc. */
	int relocMode, 				/* LLVM_SVCS_DEFAULT, LLVM_SVCS_STATIC, etc. */

	/* callbacks */
	llvm_svcs_assemble_cb_type callback,

	/* out parameters */
	string &instrBytes,			/* output bytes */
	string &strErr				/* error string */
)
{
	int rc = -1;

	/* output for asm->obj */
	SmallString<1024> smallString;
	raw_svector_ostream rsvo(smallString);

	/* source code vars */
	std::string strSrc(srcText);
	std::unique_ptr<MemoryBuffer> mbSrc;

	/*************************************************************************/
	/* the triple and the target */
	/*************************************************************************/

	// see /lib/Support/Triple.cpp for the details
	printf("triplet: %s\n", triplet);
	std::string machSpec(triplet);
	machSpec = Triple::normalize(machSpec);
	Triple TheTriple(machSpec);

	/* get the target specific parser
		if arch is blank, the triple is consulted */
	const Target *target = TargetRegistry::lookupTarget(/*arch*/"", TheTriple, strErr);
	if (!target) {
		strErr = "TargetRegistry::lookupTarget() failed\n" + strErr;
		return -1;
	}

	printf("machine spec: %s\n", machSpec.c_str());
	printf("Target.getName(): %s\n", target->getName());
	printf("Target.getShortDescription(): %s\n", target->getShortDescription());
	printf("Target.hasJIT(): %d\n", target->hasJIT());
	printf("Target.hasTargetMachine(): %d (Check if this target supports code generation)\n", target->hasTargetMachine());
	printf("Target.hasMCAsmBackend(): %d (Check if this target supports .o generation)\n", target->hasMCAsmBackend());

	/* from the target we get almost everything */
	std::unique_ptr<MCRegisterInfo> regInfo(target->createMCRegInfo(machSpec));
	if(!regInfo) {
		strErr = "creating mc register info\n";
		return -1;
	}

	std::unique_ptr<MCAsmInfo> asmInfo(target->createMCAsmInfo(*regInfo, machSpec));
	if(!asmInfo) {
		strErr = "creating MCAsmInfo\n";
		return -1;
	}

	std::unique_ptr<MCInstrInfo> instrInfo(target->createMCInstrInfo()); /* describes target instruction set */
	if(!instrInfo) {
		strErr = "creating MCInstrInfo\n";
		return -1;
	}

	std::unique_ptr<MCSubtargetInfo> subTargetInfo(target->createMCSubtargetInfo(machSpec, "", "")); /* subtarget instr set */
	if(!subTargetInfo) {
		strErr = "creating MCSubtargetInfo\n";
		return -1;
	}

	/* fixups, relaxations, objs, elfs */
	MCAsmBackend *asmBackend = target->createMCAsmBackend(*regInfo, machSpec, /* specific CPU */ "");
	if(!asmBackend) {
		strErr = "creating MCAsmBackend\n";
		return -1;
	}

	/*************************************************************************/
	/* source code manager */
	/*************************************************************************/

	// llvm::SourceMgr (include/llvm/Support/SourceMgr.h) that holds assembler source
	// has vector of llvm::SrcBuffer encaps (Support/MemoryBuffer.h) and vector of include dirs
	SourceMgr srcMgr;
	mbSrc = MemoryBuffer::getMemBuffer(strSrc);
	srcMgr.AddNewSourceBuffer(std::move(mbSrc), SMLoc());
	if(callback) srcMgr.setDiagHandler(diag_cb, (void *)callback);

	/*************************************************************************/
	/* MC context, object file, code emitter, target options */
	/*************************************************************************/

	// MC/MCObjectFileInfo.h describes common object file formats
	MCObjectFileInfo objFileInfo;

	/* MC/MCContext.h */
	MCContext context(asmInfo.get(), regInfo.get(), &objFileInfo, &srcMgr);

	/* yes, this is circular (MCContext requiring MCObjectFileInfo and visa
		versa, and is marked "FIXME" in llvm-mc.cpp */

	/* also see initMachOMCObjectFileInfo(), initELFMCObjectFileInfo(),
		initCOFFMCObjectFileInfo() ... will ask TT.getObjectFormat() if not
		specified */
	objFileInfo.InitMCObjectFileInfo(
		TheTriple,
		map_reloc_mode(relocMode),
		map_code_model(codeModel),
		context
	);

	/* code emitter llvm/MC/MCCodeEmitter.h
		has encodeInstruction() which maps MCInstr -> bytes

		target returns with X86MCCodeEmitter, ARMMCCodeEmitter, etc.
	*/
	//std::unique_ptr<MCCodeEmitter> codeEmitter(target->createMCCodeEmitter(*instrInfo, *regInfo, context));
	MCCodeEmitter *codeEmitter = target->createMCCodeEmitter(*instrInfo, *regInfo, context);
	if(!codeEmitter) {
		strErr = "creating code emitter\n";
		return -1;
	}

	/* target opts */
	MCTargetOptions targetOpts;
	targetOpts.MCUseDwarfDirectory = false;
	/* how is this different than the last field of the triplet? */
	//targetOpts.ABIName = abi;

	/*************************************************************************/
	/* assemble to object */
	/*************************************************************************/

	std::unique_ptr<MCStreamer> streamer(target->createMCObjectStreamer(
	//streamer = target->createMCObjectStreamer(
		TheTriple,
		context,
		*asmBackend,  /* (fixups, relaxation, objs and elfs) */
		rsvo, /* output stream raw_pwrite_stream */
		//codeEmitter.get(),
		codeEmitter,
		*subTargetInfo,
		true, /* relax all fixups */
		true, /* incremental linker compatible */
		false /* DWARFMustBeAtTheEnd */
	));
	//);

	if(invoke_llvm_parsers(target, &srcMgr, context, *streamer, *asmInfo,
	  *subTargetInfo, *instrInfo, targetOpts, dialect)) {
		strErr = "invoking llvm parsers\n";
		goto cleanup;
	}

	/* dump to file for debugging */
	if(0) {
		FILE *fp;
		fp = fopen("out.bin", "wb");
		fwrite(smallString.data(), 1, smallString.size(), fp);
		fclose(fp);
	}

	if(obj_output_to_bytes((uint8_t *)smallString.data(), instrBytes)) {
		strErr = "parsing bytes from LLVM obj\n";
		goto cleanup;
	}

	rc = 0;
	cleanup:
	return rc;
}

/*****************************************************************************/
/* DISASSEMBLE related functions */
/*****************************************************************************/

/* a dummy function for the disasm context, else aarch64 crashes on
	FF 43 00 D1

	see llvm-c/Disassembler.h
*/
const char *
symbol_lookup_cb(void *DisInfo, uint64_t ReferenceValue, uint64_t *ReferenceType,
	uint64_t ReferencePC, const char **ReferenceName)
{
	const char *strRefType = "unknown";

	switch(*ReferenceType) {
		/* No input reference type or no output reference type. */
		case LLVMDisassembler_ReferenceType_InOut_None:
			strRefType = "none"; break;
		/* The input reference is from a branch instruction. */
		case LLVMDisassembler_ReferenceType_In_Branch:
			strRefType = "branch"; break;
		/* The input reference is from a PC relative load instruction. */
		case LLVMDisassembler_ReferenceType_In_PCrel_Load:
			strRefType = "pcref_load"; break;

		/* The input reference is from an ARM64::ADRP instruction. */
		case LLVMDisassembler_ReferenceType_In_ARM64_ADRP:
			strRefType = "arm64_adrp"; break;
		/* The input reference is from an ARM64::ADDXri instruction. */
		case LLVMDisassembler_ReferenceType_In_ARM64_ADDXri:
			strRefType = "arm64_addxri"; break;
		/* The input reference is from an ARM64::LDRXui instruction. */
		case LLVMDisassembler_ReferenceType_In_ARM64_LDRXui:
			strRefType = "arm64_ldrxui"; break;
		/* The input reference is from an ARM64::LDRXl instruction. */
		case LLVMDisassembler_ReferenceType_In_ARM64_LDRXl:
			strRefType = "arm64_ldrxl"; break;
		/* The input reference is from an ARM64::ADR instruction. */
		case LLVMDisassembler_ReferenceType_In_ARM64_ADR:
			strRefType = "arm64_adr"; break;
	}

	//printf("%s(refval:%llx reftype:%s)\n", __func__, ReferenceValue,
	//	strRefType);

	*ReferenceType = LLVMDisassembler_ReferenceType_InOut_None;
	return NULL;
}

int
disasm_single(
	/* in parameters */
	const char *triplet,
	uint8_t *src, int src_len,
	uint64_t addr,
	/* out parameters */
	string &result, int &instrLen,
	/* internal */
	LLVMDisasmContextRef context
)
{
	int rc = -1;

	char buf[1024] = {0};

	instrLen = LLVMDisasmInstruction(
		context, /* disasm context */
		src, /* source data */
		src_len, /* length of source data */
		addr, /* address */
		buf, /* output buf */
		sizeof(buf) /* size of output buf */
	);

	if(instrLen <= 0) {
		//printf("%04X: undefined?\n", offs);
		goto cleanup;
	}

	result = buf;

	rc = 0;
	cleanup:
	return rc;
}

/* disassemble a single instruction from the start of an input buffer */
int
llvm_svcs_disasm_single(
	/* in parameters */
	const char *triplet,
	uint8_t *src, int src_len,
	uint64_t addr,
	/* out parameters */
	string &result, int &instrLen
)
{
	int rc = -1;
	LLVMDisasmContextRef context = NULL;

	/* see /lib/MC/MCDisassembler/Disassembler.h */
	context = LLVMCreateDisasm (
		triplet, /* triple */
		NULL, /* void *DisInfo */
		0, /* TagType */
		NULL, /* LLVMOpInfoCallback GetOpInfo */
		symbol_lookup_cb /* LLVMSymbolLookupCallback SymbolLookUp */
	);

	if(context == NULL) {
		//printf("ERROR: LLVMCreateDisasm()\n");
		goto cleanup;
	}

	if(0 != disasm_single(triplet, src, src_len, addr, result,
	  instrLen, context)) {
		//printf("ERROR: disasm_single()\n");
		goto cleanup;
	}

	rc = 0;
	cleanup:
	if(context) {
		LLVMDisasmDispose(context);
		context = NULL;
	}
	return rc;
}

int
llvm_svcs_disasm_lengths(
	/* in parameters */
	const char *triplet,
	uint8_t *src, int src_len,
	uint64_t addr,
	/* out parameters */
	vector<int> &lengths
)
{
	int rc = -1;
	int length;
	string result;
	LLVMDisasmContextRef context = NULL;

	/* see /lib/MC/MCDisassembler/Disassembler.h */
	//printf("disassembling with triplet: %s\n", triplet);
	context = LLVMCreateDisasm (
		triplet, /* triple */
		NULL, /* void *DisInfo */
		0, /* TagType */
		NULL, /* LLVMOpInfoCallback GetOpInfo */
		symbol_lookup_cb /* LLVMSymbolLookupCallback SymbolLookUp */
	);

	if(!context) {
		//printf("ERROR: LLVMCreateDisasm()\n");
		goto cleanup;
	}

	lengths.clear();
	for(int i=0; i<src_len; ) {
		//printf("first few bytes are: %02X %02X %02X %02X\n",
		//	src[i], src[i+1], src[i+2], src[i+3]);
		if(disasm_single(triplet, src+i, src_len-i, addr+i, result, length,
		  context)) {
			if(0) {
				printf("ERROR: disasm_single()\n");
				goto cleanup;
			}
			else {
				/* make it best effort */
				break;
			}
		}

		printf("%s(): %s has length %d\n", __func__, result.c_str(), length);

		lengths.push_back(length);
		i += length;
	}

	rc = 0;
	cleanup:
	if(context) {
		LLVMDisasmDispose(context);
		context = NULL;
	}
	return rc;
}
