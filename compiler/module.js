/*
 * module.js
 * 
 *
 * 25 jul 2022 -- 17:03 -03
 * Notes:
 */

"use strict"

const parser = require('./parser.js')

// Modules dictionary.
exports.dictionary = {}

exports.add = function (config, modName) {
    exports.dictionary[modName] = {
        compiled: true,
        data: {}
    }
}

exports.searchConst = function (modName, input) {
    if (exports.dictionary[modName]) {
        if (exports.dictionary[modName].data[input] &&
            exports.dictionary[modName].data[input].entityType == parser.EntityType.CONST)
            return exports.dictionary[modName].data[input]
    }
    return false
}

exports.addSymbol = function (modName, symName, entityType, value) {
    exports.dictionary[modName].data[symName] = {
        name: symName,
        entityType: entityType,
        value: value
    }
}

exports.searchSymbol = function (modName, symName) {
    if (exports.dictionary[modName]) {
        if (exports.dictionary[modName].data[symName])
            return exports.dictionary[modName].data[symName]
    }
}

exports.dumpSymbol = function (name, val) {
    process.stdout.write('    ' + name + ': ')
    switch (val.entityType) {
        case parser.EntityType.CONST:
            process.stdout.write('CONST: value=' + val.value + '\n')
            break
        case parser.EntityType.VARIABLE:
            process.stdout.write('VARIABLE: type=' + val.value.type +
                ', array= ' + val.value.arrayLen + ', flags= ' + val.value.flags + '\n')
            break
        default:
            process.stdout.write('<UNKNOWN ENTITY TYPE>\n')
    }
}
