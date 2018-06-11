const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css" >
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.0/jquery.min.js"></script>
<script>
var valoareBec = 0;
var thisIsSwitch = 0;
var aGlobal = 0;
var boolForState = false;
var aGlobalState2 = false;

</script>
<style>
body {
  background: url('http://subtlepatterns.com/patterns/micro_carbon.png');
}
section {
  margin: 0px auto 0;
  width: 75px;
  height: 95px;
  position: relative;
  text-align: center;
}
:active, :focus {
  outline: 0;
}
/** Font-Face **/
@font-face {
  font-family: "FontAwesome";
  src: url("fonts/fontawesome-webfont.eot");
  src: url("fonts/fontawesome-webfont.eot?#iefix") format('eot'), 
       url("fonts/fontawesome-webfont.woff") format('woff'), 
       url("fonts/fontawesome-webfont.ttf") format('truetype'), 
       url("fonts/fontawesome-webfont.svg#FontAwesome") format('svg');
  font-weight: normal;
  font-style: normal;
}
/** Styling the Button **/
button{
  font-family: "FontAwesome";
  text-shadow: 0px 1px 1px rgba(250,250,250,0.1);
  font-size: 32pt;
  display: block;
  padding: 0;
  position: relative;
  text-decoration: none;
    box-shadow: 0px 3px 0px 0px rgb(34,34,34),
          0px 7px 10px 0px rgb(17,17,17),
          inset 0px 1px 1px 0px rgba(250, 250, 250, .2), 
          inset 0px -12px 35px 0px rgba(0, 0, 0, .5);
  width: 70px;
  height: 70px;
  border: 0;
  color: rgb(37,37,37);
  border-radius: 35px;
  text-align: center;
  line-height: 79px;
  background-color: rgb(83,87,93);

  transition: color 350ms ease, text-shadow 350ms;
    -o-transition: color 350ms ease, text-shadow 350ms;
    -moz-transition: color 350ms ease, text-shadow 350ms;
    -webkit-transition: color 350ms ease, text-shadow 350ms;
}
button:before {
  content: "";
  width: 80px;
  height: 80px;
  display: block;
  z-index: -2;
  position: absolute;
  background-color: rgb(26,27,29);
  left: -5px;
  top: -2px;
  border-radius: 40px;
  box-shadow: 0px 1px 0px 0px rgba(250,250,250,0.1), 
        inset 0px 1px 2px rgba(0, 0, 0, 0.5);
}
button:active {
    box-shadow: 0px 0px 0px 0px rgb(34,34,34),
          0px 3px 7px 0px rgb(17,17,17),
          inset 0px 1px 1px 0px rgba(250, 250, 250, .2), 
          inset 0px -10px 35px 5px rgba(0, 0, 0, .5);
    background-color: rgb(83,87,93);
    top: 3px;
}
button.on {
    box-shadow: 0px 0px 0px 0px rgb(34,34,34),
          0px 3px 7px 0px rgb(17,17,17), 
          inset 0px 1px 1px 0px rgba(250, 250, 250, .2), 
          inset 0px -10px 35px 5px rgba(0, 0, 0, .5);
    background-color: rgb(83,87,93);
    top: 3px;
  color: #fff;
    text-shadow: 0px 0px 3px rgb(250,250,250);
}
button:active:before, button.on:before {
  top: -5px;
  background-color: rgb(26,27,29);
  box-shadow: 0px 1px 0px 0px rgba(250,250,250,0.1), 
        inset 0px 1px 2px rgba(0, 0, 0, 0.5);
}
/* Styling the Indicator light */
button + span {
  display: block;
  width: 8px;
  height: 8px;
  background-color: rgb(226,0,0);
  box-shadow: inset 0px 1px 0px 0px rgba(250,250,250,0.5),
        0px 0px 3px 2px rgba(226,0,0,0.5);
  border-radius: 4px;
  clear: both;
  position: absolute;
  bottom: 0;
  left: 42%;
  transition: background-color 350ms, box-shadow 700ms;
  -o-transition: background-color 350ms, box-shadow 700ms;
  -moz-transition: background-color 350ms, box-shadow 700ms;
  -webkit-transition: background-color 350ms, box-shadow 700ms;
}
button.on + span {
  box-shadow: inset 0px 1px 0px 0px rgba(250,250,250,0.5),
        0px 0px 3px 2px rgba(135,187,83,0.5);
  background-color: rgb(135,187,83);
}
</style>
</head>
<body>
<h1 class="text-center" style="color:white; margin-bottom:40px;">ESP8266 NODEMCU Light Bulb!</h1>

<section>
    <button id="button">&#xF011;</button>
    <span></span>
  </section>
    <div class ="text-center" style="color:white;">
    <br>
    ADC Value is : <span id="ADCValue">0</span><br>
    Switch value is : <span id ="Switcher">NA</span>
    LED State is : <span id="LEDState">NA</span>
  </div>
<script>
function sendData(led) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
    }
  };
  xhttp.open("POST", "setLED?LEDstate="+led, true);
  xhttp.send();
}

setInterval(function() {
  // Call a function repetatively with 2 Second interval
  getData();
  getData1();
}, 500); //2000mSeconds update rate

function getData() {
  var xhttp = new XMLHttpRequest();
//  console.log(xhttp);
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("ADCValue").innerHTML =
      this.responseText;
            document.getElementById("LEDState").innerHTML =
      this.responseText;
      aGlobal = this.responseText;
//        console.log("aGlobal value" + aGlobal);
      if(aGlobal == 1)
      {
        valoareBec = 1;
        if(aGlobalState2 == false)
        {
          $('#button').toggleClass('on');
//          console.log("triggered1");
          aGlobalState2 = true;
        }
       }
        else if (aGlobal == 0)
        {
          valoareBec = 0;
          if(aGlobalState2 == true)
          {
//          console.log("triggered2");
          $('#button').toggleClass();
          aGlobalState2 = false;
          }
        }
    }
  };
  xhttp.open("GET", "readADC", true);
  xhttp.send();
}
function getData1() {
  var xhttp = new XMLHttpRequest();
//  console.log(xhttp);
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("Switcher").innerHTML =
      this.responseText;
      thisIsSwitch = this.responseText;
//      console.log(boolForState);
      if(thisIsSwitch == 1)
      {
        if(boolForState == false){
        $('#button').toggleClass('on');
//        console.log("Triggerd");
//         $('#button').attr("onclick",sendData(thisIsSwitch));
        boolForState = true;
        }
        }
        else if( thisIsSwitch == 0)
        {
          if(boolForState == true)
          {
          $('#button').toggleClass();
//          $('#button').attr("onclick",sendData(thisIsSwitch));
  
          boolForState = false;
          }
        }
    }
  };
//  console.log("The switch value is : " + thisIsSwitch );
  xhttp.open("GET", "readSwitch", true);
  xhttp.send();
}
$(document).ready(function(){
  $('#button').click(function(){
//    $(this).toggleClass('on');
    if(valoareBec == 0)
    {
    valoareBec = 1;
    $(this).attr("onclick",sendData(valoareBec));
//    console.log(valoareBec);
  }
    else 
  {
    valoareBec = 0;
    $(this).attr("onclick",sendData(valoareBec));
//    console.log(valoareBec);
} 
  });
});
</script>
</body>
</html>
)=====";
