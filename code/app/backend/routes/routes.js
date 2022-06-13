import express from 'express'

const router = express.Router();
import {read, write, getData} from '../controllers/controllers.js'

router.get('/read', read)
router.get('/getdata', getData)
router.post('/write', write)


export default router
