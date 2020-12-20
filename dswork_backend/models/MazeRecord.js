const { ObjectId } = require('mongodb')
const logger = require('../plugins/logger')
const mongo = require('../plugins/mongodb')

class MazeRecord {
  constructor(maze, size, start, finish, algorithm) {
    this.maze = maze
    this.size = size
    this.start = start
    this.finish = finish
    this.algorithm = algorithm
    this.time = new Date()
  }

  static async createMazeRecord(maze, size, start, finish, algorithm) {
    const newMaze = new MazeRecord(maze, size, start, finish, algorithm)
    await mongo.insert('record', newMaze)
  }

  static async getAllRecord() {
    return (await mongo.find('record')).sort((a, b) => new Date(b.time) - new Date(a.time)).slice(0, 10)
  }

  static async getRecord(criteria) {
    return await mongo.find('record', { _id: ObjectId(criteria.id) })
  }
}

module.exports = MazeRecord