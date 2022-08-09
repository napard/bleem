/*
 * error.js
 * 
 *
 * 25 jul 2022 -- 17:30 -03
 * Notes:
 */

const lexer = require('./lexer.js')

exports.ErrorTypes = Object.freeze({
    TOKEN_TYPE_MISMATCH: -1,            // Token type mismatch.
    DUPLICATED_MODULE: -2,              // Trying to compile a module with same name of a prev. module.
    INVALID_SECTION: -3,                // Invalid module section for sentence.
    MISTAKE: -4,                        // Unrecognized token.
    EXPR_NOT_CONST: -5,                 // Expression is not constant.
    SYNTAX_ERROR: -6,                   // Syntax error for sentence (not enough params, etc.)
    DUPLICATED_SYMBOL: -7,              // Duplicated symbol in module.
    INVALID_OPCODE: -8,                 // Invalid opcode mnemonic.
    INVALID_SCALE: -9                   // Invaled scale vale for MOV instruction.
})

let Errors = Object.freeze([
    { msg: 'Token type mismatch' },
    { msg: 'Duplicated module' },
    { msg: 'Invalid section' },
    { msg: 'Unrecognized token' },
    { msg: 'Expression is not constant' },
    { msg: 'Syntax error' },
    { msg: 'Duplicated symbol' },
    { msg: 'Invalid opcode mnemonic' },
    { msg: 'Invalid scale value' }
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
    process.stderr.write(lexer.restoreInput(msg) + '\n    on line ' + lineIndex + ': ' + line + '\n')
    process.exit(code)
}
