/*
 * parser.js
 * 
 *
 * 25 jul 2022 -- 12:31 -03
 * Notes:
 */

"use strict"

const assembler = require('./assembler.js')
const error = require('./error.js')
const lexer = require('./lexer.js')
const _module = require('./module.js')

exports.SrcSection = Object.freeze({
    TOP: 0,
    MODULE_DECLS: 1,
    OBJECT: 2,
    FUNCTIONS: 3,
    MODULE_BODY: 4,
    MODULE_END: 5
})

exports.EntityType = Object.freeze({
    CONST: 1,
    VARIABLE: 2,
    DATA: 3
})

exports.VariableType = Object.freeze({
    CHAR: 1,
    WORD: 2
})

exports.getSectionName = function (section) {
    switch (section) {
        case exports.SrcSection.TOP: return 'TOP'
        case exports.SrcSection.MODULE_DECLS: return 'MODULE_DECLS'
        case exports.SrcSection.OBJECT: return 'OBJECT'
        case exports.SrcSection.FUNCTIONS: return 'FUNCTIONS'
        case exports.SrcSection.MODULE_BODY: return 'MODULE_BODY'
        case exports.SrcSection.MODULE_END: return 'MODULE_END'
        default: return '<UNKNOWN SECTION>'
    }
}

exports.assertSections = function (config, input, reqSections) {
    let allowed = false
    reqSections.forEach(element => {
        if (config.srcSection == element) {
            allowed = true
            return
        }
    })
    if (allowed) return
    let sections = ''
    reqSections.forEach(element => {
        sections += exports.getSectionName(element) + ' '
    });
    error.error(error.ErrorTypes.INVALID_SECTION,
        ', expected `' + sections +
        '\', found: `' + exports.getSectionName(config.srcSection) + '\'', config.lineIndex, input)
}

exports.assertParamPresent = function (config, input) {
    if (input === undefined)
        error.error(error.ErrorTypes.SYNTAX_ERROR,
            ', not enough parameters', config.lineIndex, config.line)
}

exports.assertUniqueSymbol = function (config, input) {
    if (_module.searchSymbol(config.currMod, input))
        error.error(error.ErrorTypes.DUPLICATED_SYMBOL,
            '', config.lineIndex, config.line)
}

let isAddress = function (input) {
    // TODO
}

let isConst = function (input, currModName, allowStringLit = false) {
    let res = undefined
    // Predeclared constant.
    if (res = _module.searchConst(currModName, input)) return res.value
    // Address.
    //if (res = isAddress(input)) return res
    // String.
    if (allowStringLit && lexer.isString(input)) return input
    // Number literal.
    if ((res = lexer.isNumber(input)) != NaN) return res

    return res
}

exports.getVariableTypeByName = function (token) {
    if (token[0] == '&')
        return exports.VariableType.CHAR
    return exports.VariableType.WORD
}

let resolveConstBinary = function (config, pLhs, pRhs, pBinary, pIndex, pInput) {
    switch (pBinary) {
        /*+ - | * NOT*/
        case '^__ADD__^':
            return pLhs + pRhs
        case '^__SUB__^':
            return pLhs - pRhs
        case '^__OR__^':
            return pLhs | pRhs
        case '^__MUL__^':
            return pLhs * pRhs
        // TODO: implement NOT.
        default:
            error.error(error.ErrorTypes.EXPR_NOT_CONST,
                '', config.lineIndex, pInput)

    }
}

let parseConstTerm = function (config, index, input, allowStringLit = false, triggerError = true) {
    let val = undefined
    if (input[index.index] == '^__LPAREN__^') {
        index.index++
        while (input[index.index] && input[index.index] != '^__RPAREN__^')
            val = exports.parseConstExpression(config, index, input, allowStringLit, triggerError)
        if (input[index.index] != '^__RPAREN__^' && triggerError)
            error.error(error.ErrorTypes.SYNTAX_ERROR, ', unmatched parantheses', config.lineIndex, input)
        index.index++
    } else {
        if ((val = isConst(input[index.index], config.currMod, allowStringLit)) === undefined) {
            if (triggerError) {
                if (input[index.index]) {
                    error.error(error.ErrorTypes.MISTAKE, ': `' + input[index.index] + '\'', config.lineIndex, input)
                } else
                    error.error(error.ErrorTypes.SYNTAX_ERROR, ', expected constant expression', config.lineIndex, input)
            }
        }
        index.index++
    }
    return val
}

exports.parseConstExpression = function (config, index, input, allowStringLit = false, triggerError = true) {
    // TODO: a complex expression involving binaries and parens allows
    //  extra tokens and/or opening/closing parens at the end, fix it.
    let val = parseConstTerm(config, index, input, allowStringLit, triggerError)
    let binary = {}
    while (binary = lexer.isBinary(input[index.index])) {
        index.index++
        let prevVal = val
        val = parseConstTerm(config, index, input, allowStringLit, triggerError)
        val = resolveConstBinary(config, prevVal, val, binary, index, input)
    }
    return val
}

// NOTE: Functional, STAND BY WHILE NOT FUTURE.
let parse_MODULE = function (config, input) {
    // Check parameter #1 type.
    if (!lexer.isIdentifier(input[1]))
        error.error(error.ErrorTypes.TOKEN_TYPE_MISMATCH,
            ', expected an identifier', config.lineIndex, config.line)
    config.currMod = input[1]
    // Check if there is a compiled module of same name.
    if (_module.dictionary[input[1]]) {
        if (_module.dictionary[input[1]].compiled)
            error.error(error.ErrorTypes.DUPLICATED_MODULE,
                ', there is a module with same name', config.lineIndex, input)
    }
    // Check section.
    exports.assertSections(config, input, [exports.SrcSection.TOP])
    config.srcSection++
    // Add new module to dictionary.
    _module.add(config, input[1])
    return 10000
}

// NOTE: Functional, STAND BY WHILE NOT FUTURE.
let parse_ENDMODULE = function (config, input) {
    // Check section.
    exports.assertSections(config, input, [exports.SrcSection.MODULE_BODY])
    config.srcSection++
    return 10000
}

// NOTE: Functional, STAND BY WHILE NOT FUTURE.
let parse_OBJECT = function (config, input) {
    // Check section.
    exports.assertSections(config, input, [exports.SrcSection.MODULE_DECLS])
    config.srcSection++
    return 10000
}

let parse_FUNCTIONS = function (config, input) {
    // Check section.
    //-
    /* NOTE: STAND BY WHILE NOT FUTURE */
    /*assertSections(config, input, [exports.SrcSection.OBJECT])*/
    //+
    /* NOTE: ADDED WHILE NOT FUTURE */
    exports.assertSections(config, input, [exports.SrcSection.MODULE_DECLS])
    //~
    config.srcSection++
    return 10000
}

// NOTE: Functional, STAND BY WHILE NOT FUTURE.
let parse_BEGIN = function (config, input) {
    // Check section.
    exports.assertSections(config, input, [exports.SrcSection.FUNCTIONS])
    config.srcSection++
    return 10000
}

let parse_CONST = function (config, input) {
    // Check section.
    exports.assertSections(config, input, [exports.SrcSection.MODULE_DECLS])
    // Check parameter #1 type.
    exports.assertParamPresent(config, input[1])
    if (!lexer.isIdentifier(input[1]))
        error.error(error.ErrorTypes.TOKEN_TYPE_MISMATCH,
            ', expected an identifier', config.lineIndex, input)
    // Check duplicated symbol.
    exports.assertUniqueSymbol(config, input[1])
    // Check parameter #2.
    let val = undefined
    exports.assertParamPresent(config, input[2])
    if ((val = exports.parseConstExpression(config, { index: 2 }, input)) !== undefined) {
        _module.addSymbol(config.currMod, input[1],
            exports.EntityType.CONST, val)
    } else {
        error.error(error.ErrorTypes.EXPR_NOT_CONST,
            ', at: `' + input[2] + '\'', config.lineIndex, input)
    }
    return 10000
}

let parse_VAR = function (config, input) {
    // Check section.
    exports.assertSections(config, input,
        [exports.SrcSection.MODULE_DECLS,
        exports.SrcSection.OBJECT])
    let index = 1
    do {
        // Check parameter type.
        exports.assertParamPresent(config, input[index])
        if (!(lexer.isIdentifier(input[index]) ||
            lexer.isCharVar(input[index])))
            error.error(error.ErrorTypes.TOKEN_TYPE_MISMATCH,
                ', expected valid variable name', config.lineIndex, input)
        // Check duplicated symbol.
        exports.assertUniqueSymbol(config, input[index])
        let _var = {
            entityType: exports.EntityType.VARIABLE,
            name: input[index],
            type: exports.getVariableTypeByName(input[index]),
            arrayLen: 1,
            flags: (config.srcSection == exports.SrcSection.MODULE_DECLS) ?
                1 : 2
        }
        index++
        // Check for array dimensions.
        let val = undefined
        if (input[index] == '^__LBRACK__^') {
            index++
            let indObj = { index: index }
            val = exports.parseConstExpression(config, indObj, input)
            _var.arrayLen = val
            index = indObj.index
            if (input[index] != '^__RBRACK__^')
                error.error(error.ErrorTypes.SYNTAX_ERROR, ', unmatched brackets', config.lineIndex, input)
            index++
        } else
            index--
        _module.addSymbol(config.currMod, _var.name,
            exports.EntityType.VARIABLE, _var)
        if (_var.arrayLen == 1)
            index++
        if (input[index] != '^__COMMA__^')
            break
        index++
    } while (true)
    // NOTE: if we break by something that's not '^__COMMA__^', there is
    //  possibly a syntax error.
    if (index < input.length)
        error.error(error.ErrorTypes.SYNTAX_ERROR, ', unrecognized trailing content at the end', config.lineIndex, input)
}

let commands = {
    /*'MODULE': parse_MODULE,*/ // NOTE: STAND BY WHILE NOT FUTURE
    /*'ENDMODULE': parse_ENDMODULE,*/ // NOTE: STAND BY WHILE NOT FUTURE
    /*'OBJECT': parse_OBJECT,*/ // NOTE: STAND BY WHILE NOT FUTURE
    'FUNCTIONS': parse_FUNCTIONS,
    /*'BEGIN': parse_BEGIN,*/ // NOTE: STAND BY WHILE NOT FUTURE
    'CONST': parse_CONST,
    'VAR': parse_VAR,

    'DATA': assembler.parse_DATA,
    'DATA8': assembler.parse_DATA8,
    'NOP': assembler.parse_OPCODE,
    'MOV': assembler.parse_OPCODE,
}

exports.parse = function (config, input) {
    //+
    /* NOTE: ADDED WHILE NOT FUTURE */
    config.currMod = '<DEFAULT>'
    //~

    // Make new config, deep copy.
    let localConfig = JSON.parse(JSON.stringify(config))

    if (!input.length)
        return localConfig

    //+

    /* NOTE: ADDED WHILE NOT FUTURE */

    if (localConfig.srcSection === undefined)
        localConfig.srcSection = exports.SrcSection.MODULE_DECLS

    //-

    /* NOTE: STAND BY WHILE NOT FUTURE */

    if (localConfig.srcSection === undefined)
        localConfig.srcSection = exports.SrcSection.TOP

    //~

    for (let i = 0; i < input.length; i++) {
        if (commands[input[i]]) {
            i = commands[input[i]](localConfig, input)
        } else {
            error.error(error.ErrorTypes.MISTAKE,
                ', at: `' + input[i] + '\'', config.lineIndex, config.line)
        }
    }

    return localConfig
}
