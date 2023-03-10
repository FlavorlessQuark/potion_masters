import Module from './client.js'

const wasmModule = Module({
        preRun: [],
        postRun: [],
		arguments: [window.innerHeight.toString(), window.innerWidth.toString()]
		// (function () {
		// 	const w = window.innerWidth.toString();
		// 	const h = window.innerHeight.toString();

		// 	const wArr = _malloc(w.length);
		// 	const hArr = _malloc(h.length);

		// 	stringToUTF8(w, wArr, w.length);
		// 	stringToUTF8(h, hArr, h.length);
		// 	return [wArr, hArr];
		// })
		,
        canvas: (function() {
          var canvas = document.getElementById('canvas');

          // As a default initial behavior, pop up an alert when webgl context is lost. To make your
          // application robust, you may want to override this behavior before shipping!
          // See http://www.khronos.org/registry/webgl/specs/latest/1.0/#5.15.2
          canvas.addEventListener("webglcontextlost", function(e) { alert('WebGL context lost. You will need to reload the page.'); e.preventDefault(); }, false);

          return canvas;
        })(),}).then((res) =>
		{
			window.Module = res;

			// const canvas = document.getElementById('canvas');
			// canvas.width = window.innerWidth;
			// canvas.height = window.innerHeight;
			// Module.ccall('resizeWindow', null, ['number', 'number'], [canvas.width, canvas.height]);
		});

window.Module = wasmModule;
