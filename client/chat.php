<!DOCTYPE HTML>
<html>
	<head><title>Inconvenient Chat</title>
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
			request.open("POST", "http://127.0.0.1:2048", true);
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
			location.reload();
			newMessage(username,document.getElementsByName("qbox")[0].value);
			document.getElementsByName("chatarea")[0].value+="\n"+document.getElementsByName("qbox")[0].value;
			document.getElementsByName("qbox")[0].value="";
			
		}
		
		</script>
		<input name="qbox" id="qbox"><input type="submit" id="qbutton" onclick="submit();">
		</div>
</html>		