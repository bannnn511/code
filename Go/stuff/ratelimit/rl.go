package ratelimit

// func limitApiCallLuaScript(r redis.CacheHelper, key string, capacity int) error {
// 	var limitAPICall = redis.NewScript(`
//  local keys = KEYS
//  local args = ARGV
//  local current = tonumber(redis.call("LLEN", keys[1]))
//     local capacity = tonumber(args[1])
//     if current >= capacity then
//         return redis.error_reply('too many request on flight')
//     else
//         if redis.call("EXISTS", keys[1]) == 0 then
//             redis.call("RPUSH", keys[1], keys[1])
//             redis.call("EXPIRE", keys[1], 1)
//         else
//             redis.call("RPUSHX", keys[1], keys[1])
//         end
//     end
//     return true
// `)

// 	keys := []string{key}
// 	values := []interface{}{capacity}
// 	var err error

// 	if rd, ok := r.(*redis2.RedisHelper); ok {
// 		_, err = limitAPICall.Run(rd.Client, keys, values...).Uint64()
// 	}

// 	if rd, ok := r.(*redis2.ClusterRedisHelper); ok {
// 		_, err = limitAPICall.Run(rd.Client, keys, values...).Uint64()
// 	}

// 	if err != nil {
// 		if err.Error() == "too many request on flight" {
// 			return ErrRateLimitExceeded
// 		}

// 		return err
// 	}

// 	return nil
// }
