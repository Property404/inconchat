function sendPost(url,content){
	var request = new XMLHTTPRequest()
	request.open("POST", url, aync);
	request.setRequestHeader("Content-Type","text/plain;charset=ASCII");
	request.send(content);
}