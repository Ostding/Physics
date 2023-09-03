import factory from "../wasm/physic_wrapper.js";
import { TestPhysicWasm } from "./test_physic_wasm";

import { _decorator, Component, Node } from 'cc';
const { ccclass, property } = _decorator;

import { assetManager, BufferAsset } from 'cc';


@ccclass('entry')
export class entry extends Component {
    @property(BufferAsset)
    wasmBuffer: BufferAsset = null;
    start() {
        this.loadWasm();

    }

    update(deltaTime: number) {

    }

    loadWasm() {
        let wasmBinary = new Uint8Array(this.wasmBuffer.buffer());
        factory({
            wasmBinary
        }).then((module) => {
            console.log(">>>>>module:", module);
            let test: TestPhysicWasm = new TestPhysicWasm(module);
            test.testWasm();
        });


    }
}

