-- translate to lua and run on every frame
-- if (e->type == SAPP_EVENTTYPE_KEY_DOWN) {
-- 	switch (e->key_code) {
-- 	case SAPP_KEYCODE_ESCAPE: {
-- 		sapp_request_quit();
-- 		break;
-- 	}
-- 	case SAPP_KEYCODE_Q: {
-- 		camera_rotate(state.camera, 10.0f);
-- 		break;
-- 	}
-- 	case SAPP_KEYCODE_E: {
-- 		camera_rotate(state.camera, -10.0f);
-- 		break;
-- 	}
-- 	case SAPP_KEYCODE_D: {
-- 		state.camera->pos.X += 0.01f;
-- 		break;
-- 	}
-- 	case SAPP_KEYCODE_A: {
-- 		state.camera->pos.X -= 0.01f;
-- 		break;
-- 	}
-- 	case SAPP_KEYCODE_W: {
-- 		state.camera->pos.Y += 0.01f;
-- 		break;
-- 	}
-- 	case SAPP_KEYCODE_S: {
-- 		state.camera->pos.Y -= 0.01f;
-- 		break;
-- 	}
-- 	default:
-- 		break;
-- 	}
-- }
