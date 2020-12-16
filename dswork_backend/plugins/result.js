export function trueReturn(data={}, msg='') {
  return {
    data,
    msg,
    status: true
  }
}

export function falseReturn(data={}, msg='') {
  return {
    data,
    msg,
    status: false
  }
}