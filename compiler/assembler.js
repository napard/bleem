/*
 * compiler.js
 * 
 *
 * 07 ago 2022 -- 12:47 -03
 * Notes:
 */

"use strict"

const error = require('./error.js')
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
            // This is only a guard, should not happen...
            error.error(error.ErrorTypes.INVALID_OPCODE, ', in: `' + input[0] + '\'')
            break
    }
}

// NOP

let parse_NOP = function (config, input) {
    process.stdout.write(Opcodes.NOP + ' /* ' + config.assembleAddr++ + ' */\n')
    process.stdout.write(Opcodes.NOP + ' /* ' + config.assembleAddr++ + ' */\n')
    process.stdout.write(Opcodes.NOP + ' /* ' + config.assembleAddr++ + ' */\n')
    process.stdout.write(Opcodes.NOP + ' /* ' + config.assembleAddr++ + ' */\n')
}

// MOV

let expectCloseBracket = function (token, brackType, config, input) {
    if (brackType == 1 && token !== '^__RBRACK__^')
        error.error(error.ErrorTypes.SYNTAX_ERROR, ', expected closing bracket `]\'',
            config.lineIndex, input)
    else if (brackType == 2 && token !== '^__RANGBRACK__^')
        error.error(error.ErrorTypes.SYNTAX_ERROR, ', expected closing bracket `>\'',
            config.lineIndex, input)
}

let parseOffsetAndScale = function (input, index, config) {
    let offs = undefined
    let scale = undefined
    let i = index
    if (input[i] != '^__COLON__^')
        return [i, offs, scale]
    i++
    if ((offs = parser.parseConstExpression(config, { index: i++ }, input, false, false)) === undefined)
        error.error(error.ErrorTypes.EXPR_NOT_CONST,
            ', at: `' + input[i] + '\'', config.lineIndex, input)
    if (input[i] == '^__COMMA__^' && input[i] != '^__RBRACK__^' && input[i] != '^__RANGBRACK__^') {
        i++
        if ((scale = parser.parseConstExpression(config, { index: i }, input, false, false)) === undefined)
            error.error(error.ErrorTypes.EXPR_NOT_CONST,
                ', at: `' + input[i] + '\'', config.lineIndex, input)
        if (scale % 2) {
            error.error(error.ErrorTypes.INVALID_SCALE,
                ', at: `' + input[i] + '\'', config.lineIndex, input)
        }
        i++
    }
    return [i, offs, scale]
}

let parse_MOV = function (config, input) {
    // Types: 1 = constexpr, 2 = register, 3: memref
    let fst = { type: 0 }, snd = { type: 0 }, thd = { type: 0 }
    let val = undefined, ref_reg = undefined, ref_offs = undefined, ref_scale = undefined
    let brack = 0
    let index = 1
    // Check parameter #1.
    parser.assertParamPresent(config, input[1])
    if ((val = parser.parseConstExpression(config, { index: index }, input, false, false)) === undefined) {
        // No const expr, check for register.
        if ((val = isRegister(input[index])) === undefined) {
            // Check for memory reference.
            if (input[index] != '^__LBRACK__^' && input[index] != '^__LANGBRACK__^') {
                error.error(error.ErrorTypes.SYNTAX_ERROR, ', unrecognized token',
                    config.lineIndex, input)
            } else {
                // Memory reference.
                if (input[index] == '^__LBRACK__^')
                    brack = 1
                else if (input[index] == '^__LANGBRACK__^')
                    brack = 2

                fst.type = 3
                index++
                if (ref_reg = isRegister(input[index])) {
                    // Checked base register, go for offset and scale.
                    index++
                    [index, ref_offs, ref_scale] = parseOffsetAndScale(input, index, config)
                    expectCloseBracket(input[index++], brack, config, input)
                } else if (ref_offs = parser.parseConstExpression(config, { index: index }, input, false, false)) {
                    // Check only offset.
                    index++
                    expectCloseBracket(input[index++], brack, config, input)
                }
            }
        } else {
            // Register.
            fst.type = 2
            fst.val = val
            index++
        }
    } else {
        // Const expr.
        fst.type = 1
        fst.val = val
        index++
    }
    if (input[index] != '^__COMMA__^')
        error.error(error.ErrorTypes.SYNTAX_ERROR, ', expected `,\'',
            config.lineIndex, input)
    index++

    // Check parameter #2.
    parser.assertParamPresent(config, input[1])
    if ((val = parser.parseConstExpression(config, { index: index }, input, false, false)) === undefined) {
        // No const expr, check for register.
        if ((val = isRegister(input[index])) === undefined) {
            // Check for memory reference.
            if (input[index] != '^__LBRACK__^' && input[index] != '^__LANGBRACK__^') {
                error.error(error.ErrorTypes.SYNTAX_ERROR, ', unrecognized token',
                    config.lineIndex, input)
            } else {
                if (fst.type == 3)
                    error.error(error.ErrorTypes.SYNTAX_ERROR, ', invalid second operand',
                        config.lineIndex, input)

                // Memory reference.
                if (input[index] == '^__LBRACK__^')
                    brack = 1
                else if (input[index] == '^__LANGBRACK__^')
                    brack = 2

                snd.type = 3
                index++
                if (ref_reg = isRegister(input[index])) {
                    // Checked base register, go for offset and scale.
                    index++
                    [index, ref_offs, ref_scale] = parseOffsetAndScale(input, index, config)
                    expectCloseBracket(input[index++], brack, config, input)
                } else if (ref_offs = parser.parseConstExpression(config, { index: index }, input, false, false)) {
                    // Check only offset.
                    index++
                    expectCloseBracket(input[index++], brack, config, input)
                }
            }
        } else {
            // Register.
            snd.type = 2
            snd.val = val
        }
    } else {
        error.error(error.ErrorTypes.SYNTAX_ERROR, ', invalid second operand',
            config.lineIndex, input)
    }
}


