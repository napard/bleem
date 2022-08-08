/*
 * compiler.js
 * 
 *
 * 07 ago 2022 -- 12:47 -03
 * Notes:
 */

"use strict"

const lexer = require('./lexer.js')
const parser = require('./parser.js')
const _module = require('./module.js')

let Opcodes = {
    NOP: 0,
    MOV: 1
}

exports.MACHINE_WORD_SIZE = 4

let isRegister = function (token) {
    switch (token) {
        case 'R1': return 1;
        case 'R2': return 2;
        case 'R3': return 3;
        case 'R4': return 4;
        case 'R5': return 5;
        case 'R6': return 6;
        case 'R7': return 7;
        case 'R8': return 8;
        case 'SP': return 9;
        case 'FP': return 10;
    }
}

exports.parse_DATA = function (config, input) {
    // Check section.
    // assertSections(config, input, [exports.SrcSection.OBJECT])
    // Check parameter #1 type.
    parser.assertParamPresent(config, input[1])
    if (!lexer.isIdentifier(input[1]))
        error.error(error.ErrorTypes.TOKEN_TYPE_MISMATCH,
            ', expected an identifier', config.lineIndex, input)
    // Check duplicated symbol.
    parser.assertUniqueSymbol(config, input[1])
    _module.addSymbol(config.currMod, input[1],
        parser.EntityType.DATA, config.assembleAddr)
    let index = 2
    do {
        let val = undefined
        // Check parameter type.
        parser.assertParamPresent(config, input[index])
        if ((val = parser.parseConstExpression(config, { index: index }, input, false)) === undefined) {
            error.error(error.ErrorTypes.EXPR_NOT_CONST,
                ', at: `' + input[index] + '\'', config.lineIndex, input)
        }
        process.stdout.write(val + ' /* ' + config.assembleAddr + ' */')
        config.assembleAddr += exports.MACHINE_WORD_SIZE
        index++
        if (input[index] != '^__COMMA__^')
            break
        process.stdout.write(',\n')
        index++
    } while (true)
    process.stdout.write(',\n')
    // NOTE: if we break by something that's not '^__COMMA__^', there is
    //  possibly a syntax error.
    if (index < input.length)
        error.error(error.ErrorTypes.SYNTAX_ERROR, ', unrecognized trailing content at the end', config.lineIndex, input)
}

exports.parse_DATA8 = function (config, input) {
    // Check section.
    // assertSections(config, input, [exports.SrcSection.OBJECT])
    // Check parameter #1 type.
    parser.assertParamPresent(config, input[1])
    if (!lexer.isIdentifier(input[1]))
        error.error(error.ErrorTypes.TOKEN_TYPE_MISMATCH,
            ', expected an identifier', config.lineIndex, input)
    // Check duplicated symbol.
    parser.assertUniqueSymbol(config, input[1])
    _module.addSymbol(config.currMod, input[1],
        parser.EntityType.DATA, config.assembleAddr)
    let index = 2
    do {
        let val = undefined
        // Check parameter type.
        parser.assertParamPresent(config, input[index])
        if ((val = parser.parseConstExpression(config, { index: index }, input, true)) === undefined) {
            error.error(error.ErrorTypes.EXPR_NOT_CONST,
                ', at: `' + input[index] + '\'', config.lineIndex, input)
        }
        if (typeof val !== 'string') {
            process.stdout.write(val + ' /* ' + config.assembleAddr + ' */')
            config.assembleAddr++
        } else {
            for (let i = 1; i < val.length - 1; i++) {
                process.stdout.write(val.substring(i, i + 1).charCodeAt(0) + ' /* ' + config.assembleAddr + ' */')
                if (i < val.length - 2)
                    process.stdout.write(',\n')
                config.assembleAddr++
            }
        }
        index++
        if (input[index] != '^__COMMA__^')
            break
        process.stdout.write(',\n')
        index++
    } while (true)
    process.stdout.write(',\n')
    // NOTE: if we break by something that's not '^__COMMA__^', there is
    //  possibly a syntax error.
    if (index < input.length)
        error.error(error.ErrorTypes.SYNTAX_ERROR, ', unrecognized trailing content at the end', config.lineIndex, input)
}

exports.parse_OPCODE = function (config, input) {
    // Check section.
    //parser.assertSections(config, input, [parser.SrcSection.FUNCTIONS])
    switch (input[0]) {
        case 'NOP':
            parse_NOP(config, input)
            break

        case 'MOV':
            parse_MOV(config, input)
            break

        default:
            error.error(error.ErrorTypes.INVALID_OPCODE, ', in: `' + input[0] + '\'')
            break
    }
}

// NOP

let parse_NOP = function (config, input) {
    process.stdout.write(Opcodes.NOP + ' /* ' + config.assembleAddr + ' */\n')
}

// MOV

let parse_MOV = function (config, input) {
    // Types: 1 = constexpr, 2 = register
    let fst = { type: 0 }, snd = { type: 0 }, thd = { type: 0 }
    let val = undefined, index = 1
    // Check parameter #1.
    parser.assertParamPresent(config, input[1])
    if ((val = parser.parseConstExpression(config, { index: index }, input, false, false)) === undefined) {
        // No const expr, check for register.
        if ((val = isRegister(input[index])) === undefined) {
            // Check for memory reference.
            if (input[index] != '^__LBRACK__^' && input[index] != '^__LANGBRACK__^') {

            } else {
                // Memory reference.
                let a = 0
            }
        } else {
            // Register.
            fst.type = 2
            fst.val = val
        }
    } else {
        // Const expr.
        fst.type = 1
        fst.val = val
    }
}


