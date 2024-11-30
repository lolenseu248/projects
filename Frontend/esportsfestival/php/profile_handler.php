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
        <button type="submit">Search</button>
        <input type="text" name="search" placeholder="Enter a keyword..." required>
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
        <div class="profile-button">
          <button popovertarget="profile-edit" class="edit-button">Edit</button>
          <button popovertarget="profile-delete" class="delete-button">Delete</button>
        </div>

        <!-- Profile Edit -->
        <div popover id="profile-edit" class="popover-edit">
          <form action="" method="POST">
            <div class="form-group">
              <label for="image">Profile Image:</label>
              <input type="file" id="image" name="image" accept=".jpg, .jpeg, .png">
            </div>
            <div class="form-group">
              <label for="username">Username:</label>
              <input type="username" id="username" name="username" value="<?php echo $username; ?>">
            </div>
            <div class="form-group">
              <label for="email">Email:</label>
              <input type="email" id="email" name="email" value="<?php echo $email; ?>">
            </div>
            <div class="form-group">
              <label for="password">Password:</label>
              <input type="password" id="password" name="password" placeholder="New Password">
            </div>
            <div class="form-group">
              <label for="contact">Contact:</label>
              <input type="tel" id="contact" name="contact" value="<?php echo $contact; ?>">
            </div>
            <div class="form-group">
              <label for="address">Address:</label>
              <input type="address" id="address" name="address" value="<?php echo $address; ?>">
            </div>
            <div class="form-group">
              <label for="age">Age:</label>
              <input type="number" id="age" name="age" value="<?php echo $age; ?>" min="10" max="60" maxlength="2">
            </div>
            <div class="form-group">
              <label for="birthday">B-day:</label>
              <input type="date" id="birthday" name="birthday" value="<?php echo $birthday; ?>">
            </div>
            <div class="form-group">
              <label for="role">Select a Role:</label>
              <select id="role" name="role">
                <option value="User" <?php echo ($role === 'User') ? 'selected' : ''; ?>>User</option>
                <option value="Player" <?php echo ($role === 'Player') ? 'selected' : ''; ?>>Player</option>
                <option value="Sponsor" <?php echo ($role === 'Sponsor') ? 'selected' : ''; ?>>Sponsor</option>
                <option value="Speaker" <?php echo ($role === 'Speaker') ? 'selected' : ''; ?>>Speaker</option>
              </select>
              <br><br>
            </div>
            <br><br>
            <div class="form-group">
              <label for="currentpassword">Please enter your current password to confirm.</label>
              <input type="password" id="currentpassword" name="currentpassword" placeholder="Current Password*">
            </div>
            <button type="submit" class="submit">Save</button>
            <button type="button" popovertarget="profile-edit" popovertargetaction="hide" class="edit-close">Close</button>
          </form>
        </div>

        <!-- Profile Delete -->
        <div popover id="profile-delete" class="popover-delete">
          <form action="" method="POST">
            <div class="form-group">
              <label for="currentpassword">In order to delete your account, please enter your current password!</label>
              <input type="password" id="currentpassword" name="currentpassword" placeholder="Current Password*">
            </div>
            <button type="submit" class="submit">Confirm</button>
            <button type="button" popovertarget="profile-delete" popovertargetaction="hide" class="delete-close">Close</button>
          </form>
        </div>
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