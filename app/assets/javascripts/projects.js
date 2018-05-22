function Background(id) {
  var img = document.querySelector(id);
  console.log(img);
  var imgVal = document.querySelector(id).getAttribute("data-pic");
  console.log(imgVal);
  img.style.background = imgVal;
  img.style.backgroundSize = 'cover';
  img.style.backgroundRepeat = 'no-repeat';
  img.style.backgroundPosition = 'center';
}

 Background("#pro_1");
 Background("#pro_2");
 Background("#pro_3");
 Background("#pro_4");
 Background("#pro_5");
