var ham = document.querySelector('#icon');
var sideMenu = document.querySelector('.side-nav');

function addClickHam() {
    this.classList.toggle("click-icon");
    sideMenu.classList.toggle("responsive");
}

ham.addEventListener('click', addClickHam);


  var topPos = $('#top').offset();
  var onePos = $('#1').offset();
  var twoPos = $('#2').offset();
  var threePos = $('#3').offset();
  var fourPos = $('#4').offset();
  var fivePos = $('#5').offset();
  var sixPos = $('#6').offset();
  var sevenPos = $('#7').offset();

$(document).scroll(function () {
  var y = $(this).scrollTop();

  if (y >= topPos.top) {
    $('.side a').css('background-color', 'transparent');
    $('.side a:nth-child(1)').css('background-color', 'white');
  }

  if (y >= onePos.top) {
    $('.side a').css('background-color', 'transparent');
    $('.side a:nth-child(2)').css('background-color', 'white');
  }

  if (y >= twoPos.top) {
    $('.side a').css('background-color', 'transparent');
    $('.side a:nth-child(3)').css('background-color', 'white');
  }

  if (y >= threePos.top) {
    $('.side a').css('background-color', 'transparent');
    $('.side a:nth-child(4)').css('background-color', 'white');
  }

  if (y >= fourPos.top) {
    $('.side a').css('background-color', 'transparent');
    $('.side a:nth-child(5)').css('background-color', 'white');
  }

  if (y >= fivePos.top) {
    $('.side a').css('background-color', 'transparent');
    $('.side a:nth-child(6)').css('background-color', 'white');
  }

  if (y >= sixPos.top) {
    $('.side a').css('background-color', 'transparent');
    $('.side a:nth-child(7)').css('background-color', 'white');
  }

  if (y >= sevenPos.top) {
    $('.side a').css('background-color', 'transparent');
    $('.side a:nth-child(8)').css('background-color', 'white');
  }
});

// Select all links with hashes
$('a[href*="#"]')
  // Remove links that don't actually link to anything
  .not('[href="#"]')
  .not('[href="#0"]')
  .click(function(event) {
    // On-page links
    if (
      location.pathname.replace(/^\//, '') == this.pathname.replace(/^\//, '')
      &&
      location.hostname == this.hostname
    ) {
      // Figure out element to scroll to
      var target = $(this.hash);
      target = target.length ? target : $('[name=' + this.hash.slice(1) + ']');
      // Does a scroll target exist?
      if (target.length) {
        // Only prevent default if animation is actually gonna happen
        event.preventDefault();
        $('html, body').animate({
          scrollTop: target.offset().top
        }, 1000, function() {
          // Callback after animation
          // Must change focus!
          var $target = $(target);
          $target.focus();
          if ($target.is(":focus")) { // Checking if the target was focused
            return false;
          } else {
            $target.attr('tabindex','-1'); // Adding tabindex for elements not focusable
            $target.focus(); // Set focus again
          };
        });
      }
    }
  });
