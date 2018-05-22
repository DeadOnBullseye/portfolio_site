function setProgress(id, color) {
  var circle = document.querySelector(id);
  var radius = circle.r.baseVal.value;
  var circumference = radius * 2 * Math.PI;
  circle.style.strokeDasharray = `${circumference} ${circumference}`;
  var val = document.querySelector(id).getAttribute("data-value");
  const offset = circumference - (circumference * val / 100);
  circle.style.strokeDashoffset = offset;
  circle.style.stroke = color;
}

setProgress("#html", "#e44b23");
setProgress("#css", "#563d7c");
setProgress("#js", "#f1e05a");
setProgress("#react", "#61dafb");
setProgress("#ruby", "#701516");
setProgress("#photoshop", "#b077ff");
setProgress("#i", "#ff7c00");
