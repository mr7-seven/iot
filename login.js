$(document).ready(function() {
  // Submit form using Ajax
  $('#signin-form').on('submit', function(e) {
    e.preventDefault();

    // Get form data
    var username = $('#username').val();
    var password = $('#password').val();

    // Encrypt password using MD5
    password = md5(password);

    // Send Ajax request
    $.ajax({
      url: 'signin.php',
      type: 'POST',
      data: {
        username: username,
        password: password
      },
      dataType: 'json',
      success: function(response) {
        // Handle success response
        if (response.success) {
          // User is authenticated, redirect or show success message
          alert('Sign in successful!');
        } else {
          // User authentication failed, show error message
          alert('Invalid username or password!');
        }
      },
      error: function(xhr, status, error) {
        // Handle error response
        console.error(xhr.responseText);
      }
    });
  });
});
