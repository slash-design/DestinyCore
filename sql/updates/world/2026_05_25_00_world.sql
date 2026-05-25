-- Add missing trinity_string entries for server shutdown console commands
-- LANG_SHUTDOWN_DELAYED (11017) and LANG_SHUTDOWN_CANCELLED (11018) are referenced
-- in cs_server.cpp but were never inserted into the base world database.
DELETE FROM `trinity_string` WHERE `entry` IN (11017, 11018);
INSERT INTO `trinity_string` (`entry`, `content_default`) VALUES
(11017, 'Server shutdown delayed to %d seconds as other users are still connected. Specify ''force'' to override.'),
(11018, 'Server shutdown scheduled for T+%d seconds was successfully cancelled.');
