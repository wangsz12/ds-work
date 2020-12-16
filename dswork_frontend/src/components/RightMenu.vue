<template>
  <div
    v-if="showRightMenu"
    class="right-menu--container"
    :style="`top: ${x}px; left: ${y}px;`"
    @contextmenu.prevent=""
    >
    <div class="right-menu--menu-item" @click="assign('s')">
      <i class="icon start-icon"></i>
      <span>设置为起点</span>
    </div>
    <div class="right-menu--menu-item" @click="assign('f')">
      <i class="icon finish-icon"></i>
      <span>设置为终点</span>
    </div>
  </div>
</template>

<script>
import { mapState, mapActions } from 'vuex'

export default {
  name: 'RightMenu',
  data() {
    return {
    }
  },
  props: {
    x: {
      type: Number,
      default: 0
    },
    y: {
      type: Number,
      default: 0
    }
  },
  methods: {
    ...mapActions(['setShowRightMenu', 'setCellAssignment']),

    assign(assignment) {
      this.$emit('select', assignment)
    },
    close() {
      this.setShowRightMenu(false)
    }
  },
  computed: {
    ...mapState(['showRightMenu'])
  },
  mounted() {
    document.querySelector('body').addEventListener('click', this.close)
  },
  beforeDestroy() {
    document.querySelector('body').removeEventListener('click', this.close)
  }
}
</script>

<style lang="scss" scoped>
  .right-menu--container {
    position: absolute;
    padding: .5rem 0;
    background-color: #FFF;
    border-radius: 7px;
    box-shadow: 1px 3px 15px -5px rgba(0,0,0,.4);
    z-index: 99999;

    .right-menu--menu-item {
      padding: .5rem 1rem;
      text-align: center;
      cursor: pointer;

      &:hover {
        background-color: rgb(236, 245, 255);
      }
    }

    .icon {
      display: inline-block;
      vertical-align: middle;
      height: 1rem;
      width: 1rem;
      margin-right: .5rem;
      background-size: cover;
    }

    .start-icon {
      @extend .icon;
      background-image: url(../assets/start.png);
    }

    .finish-icon {
      @extend .icon;
      background-image: url(../assets/finish.png);
    }
  }


</style>