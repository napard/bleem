/*
 * repl.js
 * 
 *
 * 28 jun 2022 -- 09:31 -03
 * Notes:
 */

"use strict"

const readline = require('readline')
const lexer = require('./lexer.js')
const parser = require('./parser.js')

const Configuration = {
    debugEnabled: true,
    transpilerEnabled: true
}

let repl = function () {
    let rl = readline.createInterface({
        input: process.stdin,
        output: process.stdout,
        terminal: false
    })

    let input = ''
    let lineIndex = 1
    rl.on('line', function (line) {
        if (line.length == 0) {
            Configuration.lineIndex = lineIndex
            parser.parse(Configuration, 0, lexer.tokenize(Configuration, input.trim()))
            input = ''
            lineIndex++
        } else {
            // Eliminate single-line comments from input line.
            line = line.replace(/\/\/.*/, '')
            input += ' %__NL__ ' + line
        }
    })
    .on('close', function (line) {
        // EOF reached, process last line if there is any.
        if(input.length) {
            Configuration.lineIndex = lineIndex
            parser.parse(Configuration, 0, lexer.tokenize(Configuration, input.trim()))
        }
    })
}

repl()
