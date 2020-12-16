const logger = require('./logger')

const MongoClient = require('mongodb').MongoClient

const DBName = 'dswork'
const host = `mongodb://localhost:27017/${DBName}`
const config = {
  useNewUrlParser: true,
  useUnifiedTopology: true
}
const collections = ['record']

MongoClient.connect(host, config)
  .then((mongo) => {
    const database = mongo.db(DBName)
    collections.forEach((item) => {
      database.listCollections({ name: item })
        .next((err, collectInfo) => {
          if (err) throw err
          if (collectInfo === null) {
            database.createCollection(item)
              .catch((err) => {
                if (err) throw err
              })
          }
      })
    })
    logger.log('database connected')
  }).catch((err) => {
    if (err) throw err
  })

async function insert(collect, item) {
  return MongoClient.connect(host, config)
    .then((mongo) => {
      const _collection = mongo.db(DBName).collection(collect)
      return _collection.insertOne(item)
        .then((res) => {
          return res.ops
        })
        .catch((err) => {
          if (err) throw err
        })
    })
}

async function find(collect, filter={}) {
  return MongoClient.connect(host, config)
    .then((mongo) => {
      const _collection = mongo.db(DBName).collection(collect)
      return _collection.find(filter).toArray() || []
    })
}

async function update(collect, filter, update) {
  update = {
    $set: update
  }
  return MongoClient.connect(host, config)
    .then((mongo) => {
      const _collection = mongo.db(DBName).collection(collect)
      return _collection.updateOne(filter, update)
        .catch((err) => {
          if (err) throw err
        })
    })
}

async function updateAll(collect, filter, update) {
  update = {
    $set: update
  }
  return MongoClient.connect(host, config)
    .then((mongo) => {
      const _collection = mongo.db(DBName).collection(collect)
      return _collection.updateMany(filter, update)
        .catch((err) => {
          if (err) throw err
        })
    })
}

async function Delete(collect, filter) {
  return MongoClient.connect(host, config)
    .then((mongo) => {
      const _collection = mongo.db(DBName).collection(collect)
      return _collection.deleteOne(filter)
        .catch((err) => {
          if (err) throw err
        })
    })
}

async function initDB() {
  return MongoClient.connect(host, config)
    .then((mongo) => {
      const db = mongo.db(DBName)
      for (let collect of collections) {
        db.dropCollection(collect)
          .catch((err) => {
            if (err) throw err
          })
      }
    })
    .catch((err) => {
      if (err) throw new Error(err)
    })
}

module.exports = {
  insert,
  find,
  update,
  updateAll,
  Delete,
  initDB
}