const std = @import("std");
const cs = @cImport({
    @cInclude("capstone/capstone.h");
});
const testing = std.testing;

test "basic open functionality" {
    var handle: cs.csh = 0;
    const res: cs.cs_err = cs.cs_open(cs.CS_ARCH_AARCH64, cs.CS_MODE_LITTLE_ENDIAN, &handle);
    try testing.expectEqual(@as(cs.cs_err, cs.CS_ERR_OK), res);
    defer _ = cs.cs_close(@constCast(&handle));
}
