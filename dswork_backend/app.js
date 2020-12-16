const Koa = require('koa')
const app = new Koa()
const fs = require('fs')
const path = require('path')

// middlewares
const bodyParser = require('koa-bodyparser')
const CORS = require('koa2-cors')
const resultLogger = require('koa-logger')
const error_haddler = require('./middlewares/error_handler')

// plugins
const logger = require('./plugins/logger')
const router = require('./plugins/controllers')
const mongo = require('./plugins/mongodb')

app.use(async (ctx, next) => {
  ctx.response.get("Access-Control-Allow-Origin", "*")
  ctx.response.set("Access-Control-Allow-Headers", "Content-Type,Content-Length,Accept,X-Requested-With,token")
  ctx.response.set("Access-Control-Allow-Methods", "PUT,POST,GET,DELETE,OPTIONS")
  if (ctx.request.method == "OPTIONS") ctx.response.status = 200
  await next()
})

app.use(resultLogger())
app.use(bodyParser())
app.use(CORS())
app.use(router.routes())
app.use(error_haddler)

router.get('/test', async (ctx, next) => {
  ctx.response.body = 'success'
  await next()
})

if (!fs.existsSync('lock')) {
  logger.log('start initializing database')
  mongo.initDB()
  fs.writeFileSync('lock', '')
  logger.log('initialization done')
}

app.listen(3000)
logger.log('server running at port 3000')