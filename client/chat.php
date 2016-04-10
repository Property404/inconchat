<!DOCTYPE HTML>
<html>
	<head><title>Inconvenient Chat</title>
 <script src="pixi.dev.js" type="text/javascript"></script>
    <script src="http://js.leapmotion.com/leap-0.4.2.js"></script>
	<link rel="stylesheet" href="style.css">
	</head>
	<body>
		<div class="chatbox">
			<textarea name="chatarea" cols="30" rows="25" readonly></textarea><br>
			
		<script>
		var username = <?php
			if(array_key_exists("username",$_POST)){
				echo("\"".$_POST["username"]."\";\n");
			}else{
				header("Location: ./");
			}
		?>
		function sendPost(content,doit){
			var request = new XMLHttpRequest()
			request.open("POST", "http://172.18.0.177:2048", true);
			request.setRequestHeader("Content-Type","text/plain;charset=ASCII");
			request.send(content);
			if (doit){
			request.onreadystatechange = function(event) {
				if (request.readyState == 4 && request.status == 200) {                
					document.getElementsByName("chatarea")[0].innerHTML=(request.responseText);
				}
			}
			}
			return "Nada";
		}
		function newUser(user){
			return sendPost("NUSR "+user,false);
		}
		function newMessage(user,message){
			return sendPost("NMSG "+user+" "+message,false);
		}
		function getMessages(){
			return sendPost("RECV I HATE BABIES",true);
		}
		newUser(username);
		getMessages();
		
		function submit(){
			newMessage(username,document.getElementsByName("qbox")[0].innerHTML);
			document.getElementsByName("qbox")[0].innerHTML="";
			location.reload();
		}
		
		</script>
		<!--
		<input name="qbox" id="qbox"><input type="submit" id="qbutton" onclick="submit();">
		-->
		<div name="qbox" id="qbox"></div>
		<div name="bbox" id="bbox"/>,</div><br>
		<input type="submit" value="add" onclick='document.getElementsByName("qbox")[0].innerHTML+=document.getElementsByName("bbox")[0].innerHTML;'>
		<input type="submit" value="send" onclick="submit();">
		<script>
		var allow_dis=false;
	var l=[0,0,0];
	var n=['+','-','<','>','[',']','.',','];
	var x=0;
	var y=0;
	var z=0;
	var lx=0;
	var ly=0;
	var lz=0;
	var r=0;
	var s = 50;
    // Set up the Leap Motion controller loop
    var options = {frameEventName: "animationFrame"};
    var controller = Leap.loop(options, function(frame) {

      // I want elements of the game to move at a consistent speed.
      // If I simply animated stuff inside of the draw loop,
      // the game speed would change depending on the frame rate.
      // I use time delta between renders to determine how far
      // elements in the scene should move in each frame.
      var now = window.performance.now();
      timer = now;

      
      if (allow_dis==true && frame.pointables.length > 0) {
		allow_dis=false;
        // Get the normalized finger 
        var pos = frame.pointables[0].stabilizedTipPosition;
        var normPos = frame.interactionBox.normalizePoint(pos, true);

        // Move the rocket to the normalized finger position
        x = Math.round(1000* normPos[0])-500;
        y = Math.round(1000* normPos[0])-500;
		z = Math.round(1000* normPos[0])-500;
		
		if(Math.abs(y-ly)>=s){
			r+=1;
			document.getElementsByName("bbox")[0].innerHTML=n[r%8];
		}
		
		lx = x;
		ly = y;
		lz = z;
		
		//document.write("\n<br>");
      }else{
		allow_dis=true;
	  }

      
    });
		</script>
	</div>
</html>		