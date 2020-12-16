const router = require('koa-router')()
const fs = require('fs')
const logger = require('./logger')
const path = require('path')

const allFiles = fs.readdirSync(path.resolve(__dirname, '../controllers'))
const jsFiles = allFiles.filter((file) => {
  return file.endsWith('.js')
})

jsFiles.forEach((jsFile) => {
  const { prefix, handlers } = require(`../controllers/${jsFile}`)
  for (let url in handlers) {
    if (url.startsWith('GET')) {
      router.get(prefix + url.substr(4), handlers[url])
    }
    else if (url.startsWith('POST')) {
      router.post(prefix + url.substr(5), handlers[url])
    }
    else {
      logger.error(`unknown API ${url}`)
      continue
    }
  }
})

logger.log('controllers loaded')

module.exports = router