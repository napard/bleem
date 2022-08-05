/*
 * compile.js
 *
 *
 * 06 jul 2022 -- 08:21 -03
 * Notes:
 */

"use strict"

const error = require('./error.js')
const lexer = require('./lexer.js')

/**
 * Transforms an expression node in its "compiled" form.
 * @param {*} expr 
 * @returns 
 */
exports.compileExpr = function (expr) {
    let finalSrc = ''

    if (expr.type == 'number') {
        finalSrc += expr.data + ' '
    } else if (expr.type == 'string') {
        if (expr.data.length == 1) {
            finalSrc += '[sm.char] ' + expr.data
        } else {
            finalSrc += 's" ' + expr.data + '"'
        }
    } else if (expr.type == 'callUnary') {
        finalSrc += expr.name + '\n'
    } else if (expr.type == 'callBinary') {
        finalSrc += expr.name + ' '
    } else if (expr.type == 'callKeyword') {
        finalSrc += '__Kw_' + expr.name.join().replace(/\:/g, '_').replace(/\,/g, '') + ' '
    } else {
        error.error(error.ErrorTypes.INTERNAL_ERROR,
            ', non compilable node:\n`' + JSON.stringify(expr, null, 4) + '\'')
    }

    return lexer.restoreBuiltin(finalSrc)
}

/**
 * Mangle lone tokens.
 * @param {*} token 
 */
exports.translateLoneToken = function (token) {
    return '__Lt_' + token + '_'
}

/**
 * Mangle unary calls.
 * @param {*} token 
 * @returns 
 */
exports.translateUnary = function (token) {
    return '__U_' + token.substring(1) + '_'
}

/**
 * Mangle custom eval tokens.
 * @param {*} token 
 * @returns 
 */
exports.translateCustomEval = function (token) {
    return '__Ev_' + token.substring(1) + '_'
}
