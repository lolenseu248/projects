<?php
// Database connection
include('connection.php');

// To store the response message
$message = ""; 

// Check if the form is submitted
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Get form data and validate inputs
    $userName = $_POST['username'] ?? '';
    $userEmail = $_POST['email'] ?? '';
    $userPassword = $_POST['password'] ?? '';
    $userAge = $_POST['age'] ?? 0;
    $userAddress = $_POST['address'] ?? '';
    $userRole = $_POST['role'] ?? '';

    // Check if the username already exists
    $sql_check = "SELECT username FROM users_data WHERE username = ?";
    $stmt_check = $conn->prepare($sql_check);
    $stmt_check->bind_param("s", $userName);
    $stmt_check->execute();
    $stmt_check->store_result();

    if ($stmt_check->num_rows > 0) {
        $message = "User already registered. Please choose another username.";
    } else {
        // Hash the password
        $userHashedPassword = password_hash($userPassword, PASSWORD_DEFAULT);

        // Prepare the insert statement
        $sql = "INSERT INTO users_data (username, email, password, age, address, role) VALUES (?, ?, ?, ?, ?, ?)";
        $stmt = $conn->prepare($sql);
        $stmt->bind_param("ssssss", $userName, $userEmail, $userHashedPassword, $userAge, $userAddress, $userRole);

        // Execute the statement
        if ($stmt->execute()) {
            $message = "Thank you for registering, " . htmlspecialchars($userName) . ".";
        } else {
            $message = "Error: " . $conn->error;
        }
    }

    // Close connection
    $stmt_check->close();
}
?>


<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=yes">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <title>Registration Status</title>
  <link rel="stylesheet" href="../css/header.css">
  <link rel="stylesheet" href="../css/registration_handler.css">
  <link rel="stylesheet" href="../css/footer.css">
  <link rel="icon" href="../img/icon.png">
</head>
<body>
  <!-- Header -->
  <div class="header">
    <a href="#" class="logo"><img src="../img/2024.png">ESports<br>Festival</a>
    <div class="header-right">
      <a href="../index.html">Home</a>
      <a href="../index2.html">About</a>
      <a href="../index3.html">Register</a>
      <a href="../index4.html">ContactUs</a>
    </div><br>
    <div class="searchbox">
      <form action="search_handler.php" method="GET">
        <button type="submit">Search</button>
        <input type="text" name="search" placeholder="Enter a keyword..." required>
      </form>
    </div>
  </div>

  <!-- Content -->
  <div class="content">
    <div class="content-box">
      <h2>Registration Status</h2>
      <?php if ($message): ?>
        <p><?php echo $message; ?></p>
      <?php endif; ?>
      <a href="../index.html">Go back to Home</a>
    </div>
  </div>

  <!-- Footer -->
  <footer>
    <h4>© Copyright 2024 | ESports Festival</h4>
  </footer>
</body>
</html>