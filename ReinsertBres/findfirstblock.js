(() => {
    let module = Process.enumerateModules()[0];
    let result = Memory.scanSync(module.base, module.size, "89 54 24 50 8D 54 24 1C 8D 4C 24 5C 89 44 24 4C E8");
    if (result.length == 0) {
        console.log("Failed to find function (1)");
        return;
    }
    let addr = result[0].address.add(result[0].size);
    result = Memory.scanSync(addr.sub(256), 256, "55 8B EC 83 E4 F8 81 EC 94 00 00 00");
    if (result.length == 0) {
        console.log("Failed to find function (2)");
        return;
    }
    let a = 1, b;
    Interceptor.attach(result[0].address, {
        onEnter(args) {
            switch (a++) {
                case 2:
                    console.log("STARTUP.TJS");
                    break;
                case 4:
                    console.log("BOOTSTRAP");
                    break;
            }
            b = 0;
        }
    });
    Interceptor.attach(addr.add(addr.readInt() + 4), {
        onEnter(args) {
            if (b++ == 0 && a % 2 == 1) {
                let arr = new Uint8Array(this.context.edx.readByteArray(64));
                console.log(arr);
            }
        }
    });
})();