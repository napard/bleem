/*
 * error.js
 *
 *
 * 28 jun 2022 -- 11:37 -03
 * Notes:
 */

"use strict"

const lexer = require('./lexer.js')

exports.ErrorTypes = Object.freeze({
    UNEXPECTED_TOKEN: -1, // (LEXERR*)
    INVALID_SYNTAX: -2, // (SYNERR*)
    INTERNAL_ERROR: -3, // (INERR*)
    KEYWORD_SELECTOR_MISMATCH: -4, // (KWERR*)
    UNIMPLEMENTED: -5
})

let Errors = Object.freeze([
    { msg: 'Unexpected token on input stream' },
    { msg: 'Invalid syntax' },
    { msg: 'INTERNAL ERROR' },
    { msg: 'Keyword selector mismatch' },
    { msg: 'UNIMPLEMENTED' },
])

/**
 * Indicate error on stderr.
 * @param {*} code 
 * @param {*} extra 
 * @param {*} lineIndex 
 * @param {*} line 
 */
exports.error = function (code, extra, lineIndex, line) {
    let msg = 'ERROR ' + '(' + code + ')' +
        ': ' + Errors[code * -1 - 1].msg + extra
    process.stderr.write(lexer.restoreBuiltin(msg) + '\n    on line: ' + line + '\n')
    process.exit(code)
}

