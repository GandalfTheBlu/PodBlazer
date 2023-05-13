
function vec2(x, y){return{x:x, y:y};}

let canvas = document.getElementById("canvas");
let screen = vec2(512, 512);
canvas.width = screen.x;
canvas.height = screen.y;
canvas.style.border = "2px solid black";
canvas.style.position = "absolute";
canvas.style.transform = "translate(-50%, -50%)";
canvas.style.left = "50%";
canvas.style.top = "50%";

let draw = canvas.getContext("2d");

function generateFont()
{
	draw.font = "64px Courier New";
	draw.fillStyle = "white";
	let chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,:!?";
	let charSize = vec2(51.2, 64);
	let yOffset = 12;
	let charCount = vec2(Math.floor(screen.x/charSize.x), Math.floor(screen.y/charSize.y));

	for(let j=0; j<charCount.y; j++)
	{
		for(let i=0; i<charCount.x; i++)
		{
			let index = j * charCount.x + i;
			if(index >= chars.length)
				return;
			
			draw.fillText(chars[index], i * charSize.x, (j + 1) * charSize.y - yOffset);
		}
	}
}

generateFont();
