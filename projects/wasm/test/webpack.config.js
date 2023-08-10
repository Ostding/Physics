const fs = require('fs');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const CopyWebpackPlugin = require('copy-webpack-plugin');

const path = require('path');
const appDirectory = fs.realpathSync(process.cwd());

module.exports = {
    entry: path.resolve(appDirectory, 'src/app.ts'),
    output: {
        filename: 'app.js',
    },
    resolve: {
        extensions: ['.ts', '.js'],
        fallback: {
            fs: false,
            os: false,
            path: false,
        }
    },
    module: {
        rules: [
            {
                test: /\.tsx?$/,
                use: 'ts-loader',
                exclude: /node_modules/,
            },
            {
                 test: /\.wasm$/,
                type: "asset/inline",
            }
        ],
    },
    plugins: [
        new HtmlWebpackPlugin({
            inject: true,
            template: path.resolve(appDirectory, 'src/index.html'),
            favicon: null
        }),
        new CopyWebpackPlugin({
            patterns: [
              { from: "wasm/physic_wrapper.wasm", to: "." }
            ],
        }),
    ],
    mode: {
        fs: "empty", // ← !!
        __dirname: false,
        Buffer: false,
        process: false,
    },
};