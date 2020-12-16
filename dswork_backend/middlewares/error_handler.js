const logger = require('../plugins/logger')
const { falseReturn } = require('../plugins/result')

async function errorHandler(ctx, next) {
  try {
    await next()
  }
  catch(err) {
    logger.error(err.message)
  }
}

module.exports = errorHandler