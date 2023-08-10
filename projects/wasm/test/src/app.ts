import wasm from "../wasm/physic_wrapper";
import { TestPhysicWasm } from './test_physic_wasm'

// init(wasm).then((wasmModule: any) => {
wasm().then((module) => {
    // Module.onRuntimeInitialized = function () {
    let test: TestPhysicWasm = new TestPhysicWasm(module);
    test.testWasm();
    // }
});

// })

