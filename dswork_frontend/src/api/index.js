import { Axios } from '../plugins/axios'

export default {
  solveMaze(payload) {
    return Axios.post('/dswork/solveMaze', payload)
  },
  getAllRecords() {
    return Axios.get('/dswork/getAllRecords')
  },
  getRecord(criteria) {
    return Axios.post('/dswork/getRecord', criteria)
  }
}