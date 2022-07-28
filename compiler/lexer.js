/*
 * lexer.js
 * 
 *
 * 25 jul 2022 -- 12:31 -03
 * Notes:
 */

"use strict"

let binaryBuiltins = {
    '&__ADD__&': '+',
    '&__SUB__&': '-',
    '&__MUL__&': '*',
    '&__DIV__&': '/',
    '&__MOD__&': ' MOD ', // Space delimited!!!
    '&__GE__&': '>=',
    '&__LE__&': '<=',
    '&__GT__&': '>',
    '&__LT__&': '<',
    '&__EQ__&': '=',
    '&__NEQ__&': '=',
    '&__AND__&': ' AND ', // Space delimited!!!
    '&__OR__&': ' OR ',
    '&__XOR__&': ' XOR ' // Space delimited!!!
}

exports.isReservedWord = function(token) {
    return (
        token === 'MOD' ||
        token === 'AND' ||
        token === 'OR' ||
        token === 'XOR' ||
        token === 'MODULE' ||
        token === 'IMPORT' ||
        token === 'CONST' ||
        token === 'VAR' ||
        token === 'OBJECT' ||
        token === 'FUNCTIONS' ||
        token === 'SET' ||
        token === 'RETURN' ||
        token === 'BEGIN' ||
        token === 'ENDMODULE')
}

exports.isIdentifier = function (token) {
    if (!token) return false
    if (token.length < 1) return false
    if (!((token[0] >= 'a' && token[0] <= 'z')
        || (token[0] >= 'A' && token[0] <= 'Z')))
        return false
    for (let i = 1; i < token.length; i++) {
        if (!((token[i] >= 'a' && token[i] <= 'z')
            || (token[i] >= 'A' && token[i] <= 'Z')
            || (token[i] >= '0' && token[i] <= '9' || token[i] == '_')))
            return false
    }
    return !exports.isReservedWord(token)
}

exports.isCharVar = function (token) {
    if (!token) return false
    if (token.length < 2) return false
    if (token[0] != '$') return false
    return exports.isIdentifier(token.substring(1))
}

exports.isNumber = function (token) {
    if (!token) return undefined
    if (token[token.length - 1] == '.')
        return undefined
    if (token.search(/\E/) > 0 && token.search(/\./) < 0)
        return undefined
    if (isNaN(token) || token[token.length - 1] == '.')
        return undefined
    return parseFloat(token)
}

exports.isBinary = function (token) {
    return Object.keys(binaryBuiltins)
        .find((str) => str === token)
}

let transform = function (input) {
    // Process exponential notation.
    let regexExp = /([0-9]+\.[0-9]*)E([\+,\-][0-9]+)/gm
    let match = regexExp.exec(input)
    while (match) {
        input = input.replace(match[0], match[1].replace('.', '&DOT&') + 'e' +
            match[2].replace('-', '&NEG&').replace('+', '&POS&'))
        match = regexExp.exec(input)
    }
    // Process simple decimal number.
    regexExp = /([0-9]+\.[0-9]*)/gm
    match = regexExp.exec(input)
    if (match) {
        input = input.replace('.', '&DOT&')
    }

    // Fix.
    input = input
        .replace(/\,/g, ' &__COMMA__& ')

        //.replace(/\%/g, ' &__FLTTYP__& ')
        //.replace(/\$/g, ' &__BYTTYP__& ')

        .replace(/\[/g, ' &__LBRACK__& ')
        .replace(/\]/g, ' &__RBRACK__& ')

        .replace(/\(/g, ' &__LPAREN__& ')
        .replace(/\)/g, ' &__RPAREN__& ')

        .replace(/\:/g, ' &__COLON__& ')
        .replace(/\!/g, ' &__BANG__& ')
        .replace(/\#/g, ' &__NUMBER__& ')
        .replace(/\@/g, ' &__AT__& ')
        .replace(/\./g, ' &__RESOL__& ')

    // Identify binary builtins.
    for (let [key, value] of Object.entries(binaryBuiltins)) {
        input = input.replace(value, ' ' + key + ' ')
    }

    // Fix exponent sign.
    input = input.replace(/\&NEG\&/g, '-')
        .replace(/\&POS\&/g, '+')
        // Fix decimal dot.
        .replace(/\&DOT\&/g, '.')

    return input
}

exports.restoreInput = function (input) {
    return input
        .replace('&__COMMA__&', ',')
        //.replace('&__FLTTYP__&', '%')
        //.replace('&__BYTTYP__&', '$')
        .replace('&__LBRACK__&', '[')
        .replace('&__RBRACK__&', ']')
        .replace('&__LPAREN__&', '(')
        .replace('&__RPAREN__&', ')')
        .replace('&__COLON__&', ':')
        .replace('&__BANG__&', '!')
        .replace('&__NUMBER__&', '#')
        .replace('&__AT__&', '@')
        .replace('&__RESOL__&', '.')
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
    out = input.split('\"')
    out = out
        .map(function (x, i) {
            if (i % 2 === 0) { // Not in string.
                return transform(x)
            } else { // Whitespace in string.
                return x.replace(/ /g, '!whitespace!')
            }
        })
        .join('\'')
        .trim()
        .split(/\s+/) // Here isolate by space.
        .map(function (x) {
            return x.replace(/!whitespace!/g, ' ')
        })
    return out
}


