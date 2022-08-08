/*
 * compiler.js
 * 
 *
 * 25 jul 2022 -- 12:24 -03
 * Notes:
 */

"use strict"

const readline = require('readline')
const error = require('./error.js')
const lexer = require('./lexer.js')
const _module = require('./module.js')
const parser = require('./parser.js')

const Configuration = {
    debugEnabled: true,
    assembleAddr: 0
}

let repl = function () {
    let rl = readline.createInterface({
        input: process.stdin,
        output: process.stdout,
        terminal: false
    })

    let lineIndex = 1
    let lastConfig = {}
    rl.on('line', function (line) {
        if (line.length > 0) {
            // Eliminate single-line comments from input line.
            line = line.replace(/\'.*/, '')

            //console.log(lexer.tokenize(Configuration, line.trim()))
            if (lineIndex == 1) {
                Configuration.lineIndex = lineIndex
                Configuration.line = line.trim()
                lastConfig = parser.parse(Configuration, lexer.tokenize(Configuration, line.trim()))
            } else {
                lastConfig.lineIndex = lineIndex
                lastConfig.line = line.trim()
                lastConfig = parser.parse(lastConfig, lexer.tokenize(lastConfig, line.trim()))
            }
            lineIndex++
        }
    })
    .on('close', function (line) {
        //-
        // Check presence of 'ENDMODULE'
        /* NOTE: STAND BY WHILE NOT FUTURE */
        /*if (lastConfig.srcSection != parser.SrcSection.MODULE_END)
            error.error(error.ErrorTypes.INVALID_SECTION,
                ', expected `ENDMODULE\'', lastConfig.lineIndex, '')*/
        //~
        // Dump debug info.
        if (lastConfig.debugEnabled) {
            console.log('MODULE: ' + lastConfig.currMod + ' ----------')
            console.log('  SYMBOL TABLE --------')
            for (let [key, value] of Object.entries(_module.dictionary[lastConfig.currMod].data)) {
                _module.dumpSymbol(key, value)
            }
        }
    })
}

repl()

