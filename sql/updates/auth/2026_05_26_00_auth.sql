-- Remove playerbot / AuctionHouseBot / ToolSocket leftovers from the auth database.

-- Playerbot tables (created by 2025_08_24_00_auth.sql / 2025_10_27_00_auth.sql).
DROP TABLE IF EXISTS `playerbot_arena`;
DROP TABLE IF EXISTS `playerbot_names`;

-- ToolSocket IP allowlist.
DROP TABLE IF EXISTS `toolip`;

-- AHBot RBAC permissions (Command: ahbot*) and the admin role's links to them.
DELETE FROM `rbac_linked_permissions` WHERE `linkedId` BETWEEN 778 AND 793;
DELETE FROM `rbac_default_permissions` WHERE `permissionId` BETWEEN 778 AND 793;
DELETE FROM `rbac_permissions` WHERE `id` BETWEEN 778 AND 793;
