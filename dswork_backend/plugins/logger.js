class Logger {

  get time() {
    const now = new Date()
    return `${now.getFullYear()}-${now.getMonth() + 1}-${now.getDate()} ${now.getHours()}:${now.getMinutes()}:${now.getSeconds()}`
  }

  log(msg) {
    console.log(`${this.time} [LOG] ${msg}`)
  }

  error(msg) {
    console.error(`${this.time} [ERROR] ${msg}`)
  }

}

const logger = new Logger()

module.exports = logger