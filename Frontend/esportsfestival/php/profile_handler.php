<?php
// Database connection
include('connection.php');

// Check if the 'id' parameter is set in the URL
if (isset($_GET['id'])) {
    $userId = $_GET['id'];

    // Fetch user data from the database based on the id
    $sql = "SELECT * FROM users_data WHERE id = ?";
    $stmt = $conn->prepare($sql);
    $stmt->bind_param("i", $userId);
    $stmt->execute();
    $result = $stmt->get_result();
    
    // Check if user exists
    if ($result->num_rows > 0) {
        $user = $result->fetch_assoc();

        // Extract user details
        // Profile image (check if available or provide a default)
        $profile_image = (!empty($user['picture'])) ? 'data:image/jpeg;base64,' . base64_encode($user['picture']) : '../img/nopic.jpg';

        $username = htmlspecialchars($user['username']);
        $role = htmlspecialchars($user['role']);
        $email = htmlspecialchars($user['email']);
        $contact = htmlspecialchars($user['contact']);
        $birthday = htmlspecialchars($user['birthday']);
        $age = htmlspecialchars($user['age']);
        $address = htmlspecialchars($user['address']);
        $created_at = htmlspecialchars($user['created_at']);
        
    } else {
        // If user doesn't exist, handle the error
        echo "User not found!";
        exit;
    }
} else {
    // If no 'id' is passed, handle the error
    echo "No user ID specified!";
    exit;
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <title>Profile</title>
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <link rel="stylesheet" href="../css/header.css">
  <link rel="stylesheet" href="../css/profile_handler.css">
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
        <input type="text" name="search" placeholder="Enter a keyword..." value="<?php echo htmlspecialchars($searchQuery); ?>" required>
        <button type="submit">Search</button>
      </form>
    </div>
  </div>

  <!-- Content -->
  <div class="content">
    <div class="content-box">
      <h2>PROFILE</h2>
      <div class="profile-container">
        <img src="<?php echo $profile_image; ?>" alt="Profile Image">
        <h2><?php echo $username; ?></h2>
      </div>
      <div class="profile-info">
        <p><strong>Role:</strong> <?php echo $role; ?></p>
        <p><strong>Email:</strong> <?php echo $email; ?></p>
        <p><strong>Contact:</strong> <?php echo $contact; ?></p>
        <p><strong>B-day:</strong> <?php echo $birthday; ?></p>
        <p><strong>Age:</strong> <?php echo $age; ?></p>
        <p><strong>Address:</strong> <?php echo $address; ?></p>
        <p><strong>Created:</strong> <?php echo $created_at; ?></p>
      </div>
    </div>
  </div>

  <!-- Footer -->
  <footer>
    <h4>© Copyright 2024 | ESports Festival</h4>
  </footer>
</body>
</html>