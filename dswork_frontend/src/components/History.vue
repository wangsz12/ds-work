<template>
  <div class="history--container">
    <div class="table">
      <el-table
        :data="tableData"
        stripe
      >
        <el-table-column
          prop="time"
          label="时间"
          align="center"
        />
        <el-table-column
          prop="size"
          label="尺寸(高×宽)"
          align="center"
        />
        <el-table-column
          prop="algorithm"
          label="所选算法"
          align="center"
        />
        <el-table-column
          prop="operation"
          label="操作"
          align="center"
        >
          <template slot-scope="record">
            <el-button type="primary" size="small" @click="viewRecord(record.row)">查看</el-button>
          </template>
        </el-table-column>
      </el-table>
    </div>
  </div>
</template>

<script>
import { mapActions } from 'vuex'
export default {
  name: 'History',
  data() {
    return {

    }
  },
  props: {
    tableData: {
      type: Array
    }
  },
  methods: {
    ...mapActions(['setMazeSize']),

    viewRecord(record) {
      this.$api.getRecord({ id: record.id })
        .then((res) => {
          if (!res.data.status) {
            return Promise.reject(res.data.msg)
          }

          const record = res.data.data.records[0]
          
          this.$emit('closeMsgBox', Promise.resolve(record))
        })
        .catch((err) => {
          this.$toast.error(err.message)
        })
    }
  }
}
</script>

<style lang="scss" scoped>
  .history--container {
    height: 100%;
    width: 100%;
    display: flex;
    justify-content: center;

    .table {
      width: 85%;
      border: rgba(0, 0, 0, .1) solid 2px;
    }
  }
</style>