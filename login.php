<?php
error_reporting(E_ALL);
ini_set('display_errors', 1);

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Handle form submission
    $username = trim($_POST['username']);
    $password = $_POST['password'];

    // Sanitize user input to prevent XSS attacks
    $username = htmlspecialchars(trim($_POST['username']));

    // Use environment variables for database credentials
    $servername = getenv('DB_SERVERNAME');
    $db_username = getenv('DB_USERNAME');
    $db_password = getenv('DB_PASSWORD');
    $db_name = getenv('DB_NAME');

    $conn = new mysqli($servername, $db_username, $db_password, $db_name);

    if ($conn->connect_error) {
        die(json_encode(['success' => false, 'message' => 'Connection failed: ' . $conn->connect_error]));
    }

    // Limit login attempts
    $login_attempts = 5;
    $login_timeout = 300; // 5 minutes in seconds
    checkLoginAttempts($username, $conn, $login_attempts, $login_timeout);

    $hashed_password = getPassword($username, $conn);

    if ($hashed_password && password_verify($password, $hashed_password)) {
        $user_rank_data = getUserRank($username, $conn);
        echo json_encode(['success' => true, 'user_rank_data' => $user_rank_data]);
    } else {
        incrementLoginAttempts($username, $conn);
        echo json_encode(['success' => false, 'message' => 'Incorrect username or password']);
    }
    $conn->close();
}

function checkLoginAttempts($username, $conn, $login_attempts, $login_timeout) {
    $stmt = $conn->prepare("SELECT last_login_attempt, login_attempts FROM xf_login_attempts WHERE username = ?");
    $stmt->bind_param("s", $username);
    $stmt->execute();
    $stmt->bind_result($last_login_attempt, $attempts);
    $stmt->fetch();
    $stmt->close();

    if ($last_login_attempt !== null) {
        $time_since_last_attempt = time() - strtotime($last_login_attempt);
        if ($attempts >= $login_attempts && $time_since_last_attempt < $login_timeout) {
            die(json_encode(['success' => false, 'message' => 'Too many failed login attempts. Please try again later.']));
        }
    }
}

function incrementLoginAttempts($username, $conn) {
    $stmt = $conn->prepare("INSERT INTO xf_login_attempts (username, login_attempts, last_login_attempt) VALUES (?, 1, NOW()) ON DUPLICATE KEY UPDATE login_attempts = login_attempts + 1, last_login_attempt = NOW()");
    $stmt->bind_param("s", $username);
    $stmt->execute();
    $stmt->close();
}

function getPassword($username, $conn) {
    $stmt = $conn->prepare("SELECT data FROM xf_user_authenticate WHERE user_id = (SELECT user_id FROM xf_user WHERE username = ?)");
    $stmt->bind_param("s", $username);
    $stmt->execute();
    $stmt->bind_result($password);
    $stmt->fetch();
    $stmt->close();

    if ($password !== null) {
        $password = substr($password, 22, -3);
        return $password;
    }
    return null;
}

function getUserRank($username, $conn) {
    $stmt = $conn->prepare("SELECT xf_user_group.title, xf_user.vip_end_time FROM xf_user_group INNER JOIN xf_user ON FIND_IN_SET(xf_user_group.user_group_id, xf_user.secondary_group_ids) OR xf_user_group.user_group_id = xf_user.user_group_id WHERE xf_user.username = ? ORDER BY xf_user_group.display_style_priority DESC LIMIT 1");
    $stmt->bind_param("s", $username);
    $stmt->execute();
    $stmt->bind_result($user_rank, $vip_end_time);
    $stmt->fetch();
    $stmt->close();

    if ($user_rank !== null) {
        if ($user_rank === 'VIP') {
            return ['user_rank' => $user_rank, 'vip_end_time' => $vip_end_time];
        }
        return ['user_rank' => $user_rank];
    }
    return null;
}
?>
