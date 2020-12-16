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
        />
      </el-table>
    </div>
  </div>
</template>

<script>
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