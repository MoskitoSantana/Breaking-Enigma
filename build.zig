const std = @import("std");

pub fn build(b: *std.build.Builder) void {
    const target = b.standardTargetOptions(.{});
    const optimization = b.standardOptimizeOption(.{});

    const bombelib = b.addSharedLibrary(.{
        .name = "bombe",
        .target = target,
        .optimize = optimization,
    });
    bombelib.linkLibCpp();
    bombelib.linkLibC();
    bombelib.force_pic = true;
    bombelib.addCSourceFiles(&.{
        "src/bombe/bombe.cpp",
        "src/Inside-Enigma/src/enigma/enigma.cpp",
        "src/Inside-Enigma/src/enigma/generator.cpp",
        "src/Inside-Enigma/src/enigma/plugboard.cpp",
        "src/Inside-Enigma/src/enigma/reflector.cpp",
        "src/Inside-Enigma/src/enigma/rotor.cpp",
    }, &.{
        "-std=c++11",
        "-Wall",
        "-W",
        "-Wstrict-prototypes",
        "-Wwrite-strings",
        "-Wno-missing-field-initializers",
    });
    b.installArtifact(bombelib);

    const exe = b.addExecutable(.{
        .name = "bombe",
        .target = target,
        .optimize = optimization,
    });
    exe.linkLibCpp();
    exe.linkLibrary(bombelib);
    exe.force_pic = true;
    exe.addCSourceFiles(&.{
        "examples/main.cpp",
    }, &.{
        "-std=c++11",
        "-Wall",
        "-W",
        "-Wstrict-prototypes",
        "-Wwrite-strings",
        "-Wno-missing-field-initializers",
    });

    b.installArtifact(exe);
}
