/*
 * metacmd.js
 * Compiler metacommands.
 *
 * 06 jul 2022 -- 19:12 -03
 * Notes:
 */

let disableTranspiler = function (config) {
    config.transpilerEnabled = false
}

let enableTranspiler = function (config) {
    config.transpilerEnabled = true
}

let showCfg = function (config) {
    console.log(JSON.stringify(config, null, 4))
}

let define = function (config) {
    disableTranspiler(config)
    process.stdout.write(': ')
}

let begin = function (config) {
    enableTranspiler(config)
}

let _return = function (config) {
    process.stdout.write('; ')
}

let test = function (config) {
    console.log('<<TEST>>\n')
}

let metaCommands = {
    'TRANSPILER_OFF': disableTranspiler,
    'TRANSPILER_ON': enableTranspiler,
    'SHOWCFG': showCfg,
    'DEFINE': define,
    'BEGIN': begin,
    'RETURN': _return,
    'TEST': test
}

/**
 * Execute a metacommand.
 * @param {*} token 
 * @param {*} config 
 */
exports.execute = function (token, config) {
    token = token.substring(1)
    if (metaCommands[token])
        metaCommands[token](config)
}
