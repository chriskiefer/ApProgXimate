function getID() {
  idCounter = localStorage.getItem('idCounter');
  if (idCounter == null) {
    idCounter = {'idx':0};
  }else{
    idCounter = JSON.parse(idCounter);
  }
  idCounter.idx++;
  localStorage.setItem('idCounter', JSON.stringify(idCounter));
  return idCounter.idx;
}

function getUrlVars() {
  var vars = {};
  var parts = window.location.href.replace(/[?&]+([^=&]+)=([^&]*)/gi, function(m,key,value) {
    vars[key] = value;
  });
  return vars;
}
