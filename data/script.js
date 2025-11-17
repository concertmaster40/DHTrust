// Function to update temperature
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      // Update temperature and add unit °c
      document.getElementById("temperature").innerHTML = this.responseText + "°c";
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ; // Poll every 10 seconds

// Function to update humidity
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      // Update humidity and add unit %
      document.getElementById("humidity").innerHTML = this.responseText + "%";
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ; // Poll every 10 seconds