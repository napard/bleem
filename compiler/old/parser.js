/*
 * parser.js
 * Parser.
 *
 * 28 jun 2022 -- 10:33 -03
 * Notes:
 */

"use strict"

const compile = require('./compile.js')
const error = require('./error.js')
const lexer = require('./lexer.js')
const metacmd = require('./metacmd.js')

let nextIndex = 0

/**
 * 
 * @param {*} config 
 * @param {*} index 
 * @param {*} input 
 */
let parseUnaries = function (config, index, input) {
    while (lexer.isUnary(input[index]) && input[index][0] == '~') {
        //if (input[index][0] != '$' && input[index][0] != '!' && input[index][0] == '~') {
        process.stdout.write(
            compile.compileExpr({ type: 'callUnary', name: compile.translateUnary(input[index]) }))
        //} else {
        //    error.error(error.ErrorTypes.UNEXPECTED_TOKEN, ': `' + input[index] + '\'', config.lineIndex, input)
        //}
        index++
    }
    nextIndex = index
}

/**
 * Keyword resolution stack.
 */
let kwStack = [{
    kwCompleteName: '',
    kwList: []
}]

/**
 * Returns top of kw resolution stack.
 * @returns 
 */
let kwStkLast = function () {
    return kwStack.length - 1
}

/**
 * Parse a binary's term.
 * @param {*} config 
 * @param {*} index 
 * @param {*} input 
 */
let parseTerm = function (config, index, input) {
    if (input[index] == '%__LPAREN__') {
        nextIndex = index + 1
        kwStack.push({
            kwCompleteName: '',
            kwList: []
        })
        while (!parseExpression(config, nextIndex, input));
        if (input[nextIndex] != '%__RPAREN__')
            error.error(error.ErrorTypes.INVALID_SYNTAX, ', unmatched parantheses', config.lineIndex, input)
        nextIndex++
        kwStack.pop()
        parseUnaries(config, nextIndex, input)
    } else {
        let atom = {}
        if (atom = lexer.isAtom(input[index])) {
            process.stdout.write(compile.compileExpr(atom))
        } else if (input[index] && input[index][0] == '!' && lexer.isIdentifier(input[index])) {
            // TODO: convert this to a call to 'compile.compileExpr(...)'. Let 'compile.compileExpr(...)'
            //  handle the case.
            process.stdout.write(compile.translateCustomEval(input[index]) + ' ')
        } else if (input[index] == '%__WILDCARD__') {
            // Nothing needed here :/
        } else {
            if (input[index]) {
                error.error(error.ErrorTypes.UNEXPECTED_TOKEN, ': `' + input[index] + '\'', config.lineIndex, input)
            } else
                error.error(error.ErrorTypes.INVALID_SYNTAX, ', expected expression', config.lineIndex, input)
        }
        nextIndex = index + 1
        parseUnaries(config, nextIndex, input)
    }
}

/**
 * Simple test 'keyword' function selectors.
 */
let testKeywordSelectors = [
    'and:',
    'openFileWithMode:',
    'createFileWithMode:',
    'to:',
    'byteTo:at:',
    'to:at:',
    'to:do:'
    // Tests:
    /*'if:',
    'else:'*/
]

/**
 * 
 * @param {*} part 
 * @param {*} index 
 * @returns 
 */
let searchKeywordPart = function (part, index) {
    for (let i = 0; i < testKeywordSelectors.length; i++) {
        if (testKeywordSelectors[i].indexOf(part, index) == index) {
            return true
        }
    }
    return false
}

/**
 * 
 * @param {*} kw 
 * @returns 
 */
let searchKeywordWhole = function (kw) {
    for (let i = 0; i < testKeywordSelectors.length; i++) {
        if (testKeywordSelectors[i] === kw)
            return true
    }
    return false
}

/**
 * Parse a whole expression.
 * @param {*} config 
 * @param {*} index 
 * @param {*} input 
 */
let parseExpression = function (config, index, input) {
    if (input[index] == '%__RPAREN__') {
        return true
    } else if (input[index] == '%__NL__') {
        process.stdout.write('\n')
        nextIndex++
        return false
    } else if (input[index] && input[index][0] == '$' && lexer.isIdentifier(input[index])) {
        metacmd.execute(input[index], config)
        nextIndex++
        return false
    } else if (input[index] && lexer.isIdentifier(input[index]) &&
        input[index][0] != '$' && input[index][0] != '~' && input[index][0] != '!') {
        process.stdout.write(compile.translateLoneToken(input[index]) + ' ')
        nextIndex++
        return false
    }
    parseTerm(config, index, input)
    let binary = {}
    while (binary = lexer.isBinary(input[nextIndex])) {
        nextIndex++
        parseTerm(config, nextIndex, input)
        process.stdout.write(compile.compileExpr({ type: 'callBinary', name: binary }))
    }
    let resolvedKw = false
    if (lexer.isKeyword(input[nextIndex])) {
        let kwPartName = ''
        while (lexer.isKeyword(input[nextIndex])) {
            resolvedKw = searchKeywordPart(input[nextIndex], kwStack[kwStkLast()].kwCompleteName.length)
            kwPartName = input[nextIndex]
            nextIndex++
            if (resolvedKw) { // Found part...
                kwStack[kwStkLast()].kwCompleteName += kwPartName
                kwStack[kwStkLast()].kwList.push(kwPartName)
                parseExpression(config, nextIndex, input)
            } else
                error.error(error.ErrorTypes.KEYWORD_SELECTOR_MISMATCH,
                    ', for: `' + kwStack[kwStkLast()].kwCompleteName + kwPartName + '\' (KWERR1)', config.lineIndex, input)
        }
        if (searchKeywordWhole(kwStack[kwStkLast()].kwCompleteName)) {
            kwStack[kwStkLast()].kwCompleteName = ''
            process.stdout.write(compile.compileExpr({ type: 'callKeyword', name: kwStack[kwStkLast()].kwList }))
            kwStack[kwStkLast()].kwList = []
        }
    }
    return false
}

exports.parse = function (config, index, input) {
    if (!input.length)
        return
    //console.log('Tokens: ' + input)

    nextIndex = index
    for (let i = nextIndex; i < input.length; i = nextIndex) {
        if (config.transpilerEnabled || (input[i] && input[i][0] == '$'))
            parseExpression(config, i, input)
        else {
            if (input[i] == '%__NL__')
                process.stdout.write('\n')
            else
                process.stdout.write(input[i] + ' ')
            nextIndex++
        }
    }
}
