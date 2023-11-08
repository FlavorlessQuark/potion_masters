  const url_arg_str = window.location.search;
  const url_params = new URLSearchParams(url_arg_str);
  const subid = url_params.get('subid');
  const box_ip = window.location.href.split('/')[2].split(':')[0];

  ws = new WebSocket("ws://" + box_ip + ":50079");

  // wait for websocket to connect
  ws.onopen = (event) => {

      console.log("openned websocket")

      byte_array = new Uint8Array(1);
      byte_array[0] = subid;
      ws.send(byte_array.buffer);

      ws.addEventListener('message', (event) => {
          console.log(event.data);
          document.getElementById("statusText").innerHTML = event.data;
      });

  }

	function testSend() {
		console.log("send");
	}

	async function sendToWasm(message)
	{
		const byteCount = Module.lengthBytesUTF8(message) + 1;
		const messagePointer =  Module._malloc(byteCount);
		Module.stringToUTF8(message, messagePointer, byteCount);
		await Module.functionName(messagePointer);

		Module._free(messagePointer);
	}

	function handleClick() {
	console.log(Module.ccall, Module._test_receive, Module.test_receive);
	sendToWasm("Message")
	// Module._test_receive("Message");
	// test("Message")
    //   ws.send("press");
  }

