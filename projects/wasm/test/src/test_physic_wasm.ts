export class TestPhysicWasm {
    private module: any = null;

    constructor(module: any) {
        this.module = module;
    }

    public testWasm() {
        console.log("3333333")
        let a = BigInt(123);
        let b = BigInt(456);
        let arr = new Uint8Array(new BigUint64Array([a, b]).buffer);
        let x = this.module.ccall("Func", "number", ['array', 'number'], [arr, arr.length]);
        console.log("Func return:", x, typeof (x));
        this.module.ccall("Func1", "null", ['number', 'string'], [a, "I'm test string!"]);
        let sum = this.module._FuncAdd(a, b);
        console.log("Func2 return:", sum, typeof (sum));
    }
}
