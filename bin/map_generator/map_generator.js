
function vec2(x, y){return{x:x, y:y};}

let canvas = document.getElementById("canvas");
let screen = vec2(800, 800);
canvas.width = screen.x;
canvas.height = screen.y;
canvas.style.border = "2px solid black";
canvas.style.position = "absolute";
canvas.style.transform = "translate(-50%, -50%)";
canvas.style.left = "50%";
canvas.style.top = "50%";

let scaleText = document.getElementById("scale");
scaleText.style.fontFamily = "Courier New";
scaleText.style.fontSize = "16px";

let button = document.getElementById("printButton");

let draw = canvas.getContext("2d");

class Line
{
	constructor(start)
	{
		this.start = start;
		this.end = vec2(0, 0);
	}
}

let lines = [];
let currentLine = null;

canvas.addEventListener("mousedown", (e)=>{
	let x = e.clientX - canvas.offsetLeft;
	let y = window.innerHeight - e.clientY - canvas.offsetTop;
	
	if(currentLine == null)
	{
		currentLine = new Line(vec2(x, y));
	}
	else
	{
		currentLine.end = vec2(x, y);
		lines.push(currentLine);
		currentLine = new Line(vec2(x, y));
	}
});

function downloadCoordinates(scale)
{
	let txt = "";
	let sx = scale / screen.x;
	let sy = scale / screen.y;
	for(let i=0; i<lines.length; i++)
	{
		txt += "(" + (lines[i].start.x * sx) + ", " + (lines[i].start.y * sy) + ")\n";
	}
	
	let e = document.createElement('a');
	e.setAttribute("href", "data:text/plain;charset=utf-8," + encodeURIComponent(txt));
	e.setAttribute("download", "map.txt");
	e.style.display = "none";
	document.body.appendChild(e);
	e.click();
	document.body.removeChild(e);
}

document.addEventListener("keydown", (e)=>{
	if(e.key == "End" && currentLine != null && lines.length > 0)
	{
		currentLine.end = lines[0].start;
		lines.push(currentLine);
		currentLine = null;
	}
});

button.addEventListener("click", (e)=>{
	let scale = parseFloat(scaleText.value);
	if(!isNaN(scale))
	{
		downloadCoordinates(scale);
	}
});

function drawLine(a, b, col)
{
	draw.beginPath();
	draw.moveTo(screen.x*0.5 + a.x, 0.5*screen.y - a.y);
	draw.lineTo(screen.x*0.5 + b.x, 0.5*screen.y - b.y);
	draw.strokeStyle = col;
	draw.stroke();
	draw.closePath();
}

function loop()
{
	draw.clearRect(0, 0, screen.x, screen.y);
	draw.fillStyle = "rgb(240, 240, 240)";
	draw.fillRect(0, 0, screen.x, screen.y);
	
	drawLine(vec2(-screen.x*0.5, 0), vec2(screen.x*0.5, 0), "red");
	drawLine(vec2(0, -screen.y*0.5), vec2(0, screen.y*0.5), "green");
	
	for(let i=0; i<lines.length; i++)
	{
		drawLine(lines[i].start, lines[i].end, "black");
	}
	
	window.requestAnimationFrame(loop);
}
loop();