/*
 * lexer.js
 * Lexer.
 *
 * 28 jun 2022 -- 09:49 -03
 * Notes:
 */

"use strict"

let binaryBuiltins = {
    '%__ADD__': '+',
    '%__SUB__': '-',
    '%__MUL__': '*',
    '%__DIV__': '/',
    '%__NEQ__': '<>',
    '%__GE__': '>=',
    '%__LE__': '<=',
    '%__GT__': '>',
    '%__LT__': '<',
    '%__EQ__': '='
}

/**
 * Indicates if token is a valid number.
 * @param {*} token 
 * @returns 
 */
exports.isNumber = function (token) {
    if (!token) return false
    if (token[token.length - 1] == '.')
        return false
    if (token.search(/\E/) > 0 && token.search(/\./) < 0)
        return false
    return !isNaN(token) && token[token.length - 1] != '.'
}

/**
 * Parse a number.
 * @param {*} input 
 */
exports.parseNumber = function (input) {
    // TODO: don't analize number literal for now.
    return parseFloat(input)
}

/**
 * Indicates if token is a valid binary function.
 * @param {*} token 
 * @returns 
 */
exports.isBinary = function (token) {
    return Object.keys(binaryBuiltins)
        .find((str) => str === token)
}

/**
 * Indicates if token is a valid identifier.
 * @param {*} token 
 * @returns 
 */
exports.isIdentifier = function (token) {
    if (!token) return false
    if (token.length < 1) return false
    if (token.length < 2 && (token[0] == '~' || token[0] == '!' || token[0] == '$')) return false
    if (!((token[0] >= 'a' && token[0] <= 'z')
        || (token[0] >= 'A' && token[0] <= 'Z') ||
        token[0] == '~' || token[0] == '$' || token[0] == '!'))
        return false
    // TODO: this would accept a metacommand or lone token which starts with a number.
    for (let i = 1; i < token.length; i++) {
        if (!((token[i] >= 'a' && token[i] <= 'z')
            || (token[i] >= 'A' && token[i] <= 'Z')
            || (token[i] >= '0' && token[i] <= '9' || token[i] == '_')))
            return false
    }
    return true
}

/**
 * Indicates if token is a valid keyword function part.
 * @param {*} token 
 * @returns 
 */
exports.isKeyword = function (token) {
    if (!token) return false
    if (token.length < 2) return false
    if (!((token[0] >= 'a' && token[0] <= 'z')
        || (token[0] >= 'A' && token[0] <= 'Z')))
        return false
    if (token.length >= 3) {
        let i = 1
        for (i = 1; i < token.length - 1; i++) {
            if (!((token[i] >= 'a' && token[i] <= 'z')
                || (token[i] >= 'A' && token[i] <= 'Z')
                || (token[i] >= '0' && token[i] <= '9')))
                return false
        }
        if (token[i] != ':') return false
    } else {
        if (token[1] != ':') return false
    }
    return true
}

/**
 * Indicates if str is a valid string literal.
 * @param {*} str 
 * @returns 
 */
exports.isString = function (str) {
    if (!str) return false
    if (str.length < 2)
        return false
    if (str[0] != '\'')
        return false
    if (str[str.length - 1] != '\'')
        return false
    return true
}

/**
 * Indicates if token can be an atom.
 * @param {*} token 
 * @returns 
 */
exports.isAtom = function (token) {
    if (exports.isNumber(token))
        return { type: 'number', data: exports.parseNumber(token) }
    if (exports.isString(token))
        return { type: 'string', data: token.substr(1, token.length - 2) }
}

/**
 * Indicates if token is a valid name for unary function.
 * @param {*} token 
 * @returns 
 */
exports.isUnary = function (token) {
    return exports.isIdentifier(token)
}

/**
 * Replace builtins with internal symbols and isolate
 *  exponential notation and keywords.
 * @param {*} input 
 * @returns 
 */
let transformBuiltin = function (input) {
    // Process exponential notation.
    let regexExp = /([0-9]+\.[0-9]*)E([\+,\-][0-9]+)/gm
    let match = regexExp.exec(input)
    while (match) {
        input = input.replace(match[0], match[1].replace('.', '$DOT$') + 'e' +
            match[2].replace('-', '$NEG$').replace('+', '$POS$'))
        match = regexExp.exec(input)
    }
    // Process simple decimal number.
    regexExp = /([0-9]+\.[0-9]*)/gm
    match = regexExp.exec(input)
    if (match) {
        input = input.replace('.', '$DOT$')
    }
    // Process keywords.
    regexExp = /([a-zA-Z]+):/gm
    match = regexExp.exec(input)
    while (match) {
        input = input.replace(match[0], '$KWS$' + match[1] + '$KWE$')
        match = regexExp.exec(input)
    }
    // Process rest.
    input = input
        .replace(/\(/g, ' %__LPAREN__ ')
        .replace(/\)/g, ' %__RPAREN__ ')
        .replace(/\./g, ' %__STOP__ ')
        .replace(/\+/g, ' %__ADD__ ')
        .replace(/\-/g, ' %__SUB__ ')
        .replace(/\*/g, ' %__MUL__ ')
        .replace(/\//g, ' %__DIV__ ')
        .replace(/\<\>/g, ' %__NEQ__ ')
        .replace(/\>\=/g, ' %__GE__ ')
        .replace(/\<\=/g, ' %__LE__ ')
        .replace(/\>/g, ' %__GT__ ')
        .replace(/\</g, ' %__LT__ ')
        .replace(/\=/g, ' %__EQ__ ')
        // Fix exponent sign.
        .replace(/\$NEG\$/g, '-')
        .replace(/\$POS\$/g, '+')
        // Fix decimal dot.
        .replace(/\$DOT\$/g, '.')
        // Fix keywords.
        .replace(/\$KWS\$/g, ' ')
        .replace(/\$KWE\$/g, ':')
        // Isolate wildcard.
        .replace(/\?/g, ' %__WILDCARD__ ')
    return input
}

/**
 * Restore true form of mangled tokens by the lexer.
 * @param {*} input 
 */
exports.restoreBuiltin = function (input) {
    return input
        .replace('%__ADD__', '+')
        .replace('%__SUB__', '-')
        .replace('%__MUL__', '*')
        .replace('%__DIV__', '/')
        .replace('%__NEQ__', '<>')
        .replace('%__GE__', '>=')
        .replace('%__LE__', '<=')
        .replace('%__GT__', '>')
        .replace('%__LT__', '<')
        .replace('%__EQ__', '=')
}

/**
 * Tokenize input stream.
 * @param {*} config 
 * @param {*} input 
 * @returns 
 */
exports.tokenize = function (config, input) {
    if (!input.length)
        return input
    let out = ''
    if (config.transpilerEnabled) {
        out = input.split('\'')
        out = out
            .map(function (x, i) {
                if (i % 2 === 0) { // Not in string.
                    return transformBuiltin(x)
                } else { // Whitespace in string.
                    return x.replace(/ /g, '!whitespace!')
                }
            })
            .join('\'')
            .trim()
            .split(/\s+/)
            .map(function (x) {
                return x.replace(/!whitespace!/g, ' ')
            })
    } else {
        out = input.split(/\s+/)
    }
    return out
}

