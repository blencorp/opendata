/**
 * Check all fields function
 */
function checkall(fields) {
  var checkall = document.getElementsByClassName('checkall_checkbox')[0].checked;
  var elem = document.getElementsByClassName(fields);
  var len = elem.length;

  if (checkall == true) {
    for (i = 0; i < len; i++)
      elem[i].checked = true;
  }
  else if (checkall == false) {
    for (i = 0; i < len; i++)
      elem[i].checked = false;
  }
}

