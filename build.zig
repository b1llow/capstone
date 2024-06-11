const std = @import("std");

// Although this function looks imperative, note that its job is to
// declaratively construct a build graph that will be executed by an external
// runner.
pub fn build(b: *std.Build) void {
    // Standard target options allows the person running `zig build` to choose
    // what target to build for. Here we do not override the defaults, which
    // means any target is allowed, and the default is native. Other options
    // for restricting supported target set are available.
    const target = b.standardTargetOptions(.{});

    // Standard optimization options allow the person running `zig build` to select
    // between Debug, ReleaseSafe, ReleaseFast, and ReleaseSmall. Here we do not
    // set a preferred release mode, allowing the user to decide how to optimize.
    const optimize = b.standardOptimizeOption(.{});

    const libcapstone = b.addStaticLibrary(.{
        .name = "capstone",
        .target = target,
        .optimize = optimize,
    });

    libcapstone.addCSourceFiles(.{ .files = &.{
        "MCInst.c",
        "MCInstPrinter.c",
        "MCInstrDesc.c",
        "MCRegisterInfo.c",
        "Mapping.c",
        "SStream.c",
        "cs.c",
        "utils.c",
        "arch/M68K/M68KInstPrinter.c",
        "arch/M68K/M68KModule.c",
        "arch/M68K/M68KDisassembler.c",
        "arch/MOS65XX/MOS65XXModule.c",
        "arch/MOS65XX/MOS65XXDisassembler.c",
        "arch/M680X/M680XModule.c",
        "arch/M680X/M680XInstPrinter.c",
        "arch/M680X/M680XDisassembler.c",
        "arch/Xtensa/XtensaModule.c",
        "arch/Xtensa/XtensaMapping.c",
        "arch/Xtensa/XtensaInstPrinter.c",
        "arch/Xtensa/XtensaDisassembler.c",
        "arch/TMS320C64x/TMS320C64xInstPrinter.c",
        "arch/TMS320C64x/TMS320C64xDisassembler.c",
        "arch/TMS320C64x/TMS320C64xModule.c",
        "arch/TMS320C64x/TMS320C64xMapping.c",
        "arch/PowerPC/PPCDisassembler.c",
        "arch/PowerPC/PPCMapping.c",
        "arch/PowerPC/PPCModule.c",
        "arch/PowerPC/PPCInstPrinter.c",
        "arch/XCore/XCoreInstPrinter.c",
        "arch/XCore/XCoreMapping.c",
        "arch/XCore/XCoreModule.c",
        "arch/XCore/XCoreDisassembler.c",
        "arch/HPPA/HPPAInstPrinter.c",
        "arch/HPPA/HPPAModule.c",
        "arch/HPPA/HPPAMapping.c",
        "arch/HPPA/HPPADisassembler.c",
        "arch/WASM/WASMModule.c",
        "arch/WASM/WASMInstPrinter.c",
        "arch/WASM/WASMMapping.c",
        "arch/WASM/WASMDisassembler.c",
        "arch/Mips/MipsInstPrinter.c",
        "arch/Mips/MipsMapping.c",
        "arch/Mips/MipsDisassembler.c",
        "arch/Mips/MipsModule.c",
        "arch/SH/SHDisassembler.c",
        "arch/SH/SHInstPrinter.c",
        "arch/SH/SHModule.c",
        "arch/TriCore/TriCoreDisassembler.c",
        "arch/TriCore/TriCoreMapping.c",
        "arch/TriCore/TriCoreModule.c",
        "arch/TriCore/TriCoreInstPrinter.c",
        "arch/X86/X86Disassembler.c",
        "arch/X86/X86ATTInstPrinter.c",
        "arch/X86/X86Module.c",
        "arch/X86/X86DisassemblerDecoder.c",
        "arch/X86/X86InstPrinterCommon.c",
        "arch/X86/X86IntelInstPrinter.c",
        "arch/X86/X86Mapping.c",
        "arch/EVM/EVMModule.c",
        "arch/EVM/EVMDisassembler.c",
        "arch/EVM/EVMInstPrinter.c",
        "arch/EVM/EVMMapping.c",
        "arch/ARM/ARMInstPrinter.c",
        "arch/ARM/ARMBaseInfo.c",
        "arch/ARM/ARMDisassemblerExtension.c",
        "arch/ARM/ARMModule.c",
        "arch/ARM/ARMMapping.c",
        "arch/ARM/ARMDisassembler.c",
        "arch/RISCV/RISCVMapping.c",
        "arch/RISCV/RISCVModule.c",
        "arch/RISCV/RISCVInstPrinter.c",
        "arch/RISCV/RISCVDisassembler.c",
        "arch/SystemZ/SystemZInstPrinter.c",
        "arch/SystemZ/SystemZModule.c",
        "arch/SystemZ/SystemZMCTargetDesc.c",
        "arch/SystemZ/SystemZDisassembler.c",
        "arch/SystemZ/SystemZMapping.c",
        "arch/Alpha/AlphaMapping.c",
        "arch/Alpha/AlphaInstPrinter.c",
        "arch/Alpha/AlphaDisassembler.c",
        "arch/Alpha/AlphaModule.c",
        "arch/AArch64/AArch64DisassemblerExtension.c",
        "arch/AArch64/AArch64Mapping.c",
        "arch/AArch64/AArch64Disassembler.c",
        "arch/AArch64/AArch64InstPrinter.c",
        "arch/AArch64/AArch64BaseInfo.c",
        "arch/AArch64/AArch64Module.c",
        "arch/BPF/BPFModule.c",
        "arch/BPF/BPFDisassembler.c",
        "arch/BPF/BPFInstPrinter.c",
        "arch/BPF/BPFMapping.c",
        "arch/Sparc/SparcModule.c",
        "arch/Sparc/SparcInstPrinter.c",
        "arch/Sparc/SparcDisassembler.c",
        "arch/Sparc/SparcMapping.c",
    } });
    var arch_iter = std.mem.splitSequence(u8, "ARM AARCH64 M68K MIPS PPC SPARC SYSZ XCORE X86 TMS320C64X M680X EVM MOS65XX WASM BPF RISCV SH TRICORE ALPHA HPPA XTENSA", " ");
    var buf: [128]u8 = undefined;
    while (arch_iter.next()) |entry| {
        const _support = std.fmt.bufPrint(&buf, "CAPSTONE_{s}_SUPPORT", .{entry}) catch unreachable;
        libcapstone.root_module.addCMacro(_support, "1");

        const has_arch = std.fmt.bufPrint(&buf, "CAPSTONE_HAS_{s}", .{entry}) catch unreachable;
        libcapstone.root_module.addCMacro(has_arch, "1");
    }
    libcapstone.root_module.addCMacro("CAPSTONE_USE_SYS_DYN_MEM", "1");

    libcapstone.addIncludePath(b.path("include"));
    libcapstone.installHeadersDirectory(b.path("include"), "", .{});
    libcapstone.linkLibC();

    const libcsprint = b.addStaticLibrary(.{
        .name = "csprint",
        .target = target,
        .optimize = optimize,
    });
    libcsprint.addCSourceFiles(.{ .files = &.{
        "cstool//cstool_alpha.c",
        "cstool//cstool_evm.c",
        "cstool//cstool_x86.c",
        "cstool//cstool_mos65xx.c",
        "cstool//cstool_bpf.c",
        "cstool//cstool_arm.c",
        "cstool//cstool_tms320c64x.c",
        "cstool//cstool_sparc.c",
        "cstool//cstool_powerpc.c",
        "cstool//cstool_systemz.c",
        "cstool//cstool_sh.c",
        "cstool//cstool_m680x.c",
        "cstool//cstool_mips.c",
        "cstool//cstool_aarch64.c",
        "cstool//cstool_m68k.c",
        "cstool//cstool_xtensa.c",
        "cstool//cstool_hppa.c",
        "cstool//cstool_riscv.c",
        "cstool//cstool_xcore.c",
        "cstool//cstool_print.c",
        "cstool//cstool_wasm.c",
        "cstool//cstool_tricore.c",
    } });
    libcsprint.addIncludePath(b.path("cstool"));
    libcsprint.installHeader(b.path("cstool/cstool.h"), "cstool.h");
    libcsprint.installLibraryHeaders(libcapstone);
    libcsprint.linkLibC();
    libcsprint.linkLibrary(libcapstone);

    const cstool = b.addExecutable(.{
        .name = "cstool",
        .target = target,
        .optimize = optimize,
    });
    cstool.addCSourceFiles(.{ .files = &.{
        "cstool//getopt.c",
        "cstool//cstool.c",
    } });
    cstool.linkLibC();
    cstool.linkLibrary(libcsprint);

    // This declares intent for the library to be installed into the standard
    // location when the user invokes the "install" step (the default step when
    // running `zig build`).
    b.installArtifact(libcapstone);
    // This declares intent for the executable to be installed into the
    // standard location when the user invokes the "install" step (the default
    // step when running `zig build`).
    b.installArtifact(cstool);
    b.installArtifact(libcsprint);

    // This *creates* a Run step in the build graph, to be executed when another
    // step is evaluated that depends on it. The next line below will establish
    // such a dependency.
    const run_cmd = b.addRunArtifact(cstool);

    // By making the run step depend on the install step, it will be run from the
    // installation directory rather than directly from within the cache directory.
    // This is not necessary, however, if the application depends on other installed
    // files, this ensures they will be present and in the expected location.
    run_cmd.step.dependOn(b.getInstallStep());

    // This allows the user to pass arguments to the application in the build
    // command itself, like this: `zig build run -- arg1 arg2 etc`
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    // This creates a build step. It will be visible in the `zig build --help` menu,
    // and can be selected like this: `zig build run`
    // This will evaluate the `run` step rather than the default, which is "install".
    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);

    // Creates a step for unit testing. This only builds the test executable
    // but does not run it.
    const lib_unit_tests = b.addTest(.{
        .root_source_file = b.path("src/root.zig"),
        .target = target,
        .optimize = optimize,
    });
    lib_unit_tests.linkLibrary(libcapstone);
    lib_unit_tests.linkLibrary(libcsprint);
    // lib_unit_tests.addIncludePath() ∏

    const run_lib_unit_tests = b.addRunArtifact(lib_unit_tests);

    // Similar to creating the run step earlier, this exposes a `test` step to
    // the `zig build --help` menu, providing a way for the user to request
    // running the unit tests.
    const test_step = b.step("test", "Run unit tests");
    test_step.dependOn(&run_lib_unit_tests.step);
}
