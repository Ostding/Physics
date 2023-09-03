import factory from "../wasm/physic_wrapper.js";
import { TestPhysicWasm } from "./test_physic_wasm";

import { _decorator, Component, Node } from 'cc';
const { ccclass, property } = _decorator;

import { assetManager, BufferAsset } from 'cc';


@ccclass('entry')
export class entry extends Component {
    @property(BufferAsset)
    wasmBuffer: BufferAsset = null;

    wasmModule: any = null;
    start() {
        this.loadWasm().then((module) => {
            this.wasmModule = module;
            let test: TestPhysicWasm = new TestPhysicWasm(module);
            test.testWasm();
        })

    }

    update(deltaTime: number) {

    }

    loadWasm(): Promise<any> {
        return new Promise((resolve, reject) => {
            try {
                let wasmBinary = new Uint8Array(this.wasmBuffer.buffer());
                factory({
                    wasmBinary
                }).then((module) => {
                    resolve(module);
                });
            } catch (error) {
                reject('init wasm failed!')
            }
        })
    }
}

