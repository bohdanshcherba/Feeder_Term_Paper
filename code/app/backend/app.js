import express from  'express'
import {SerialPort} from "serialport";
import {ReadlineParser} from "serialport"
import cors from 'cors'
import bodyParser from 'body-parser'
import router from './routes/routes.js'
import {db} from "./service/db.js";

const app = express()
const port = 8000

app.use(cors({origin: 'http://localhost:3000'}));
app.use(express.json());
app.use(express.urlencoded());
app.use('',router)

app.listen(port, () => {
  console.log(`Example app listening on port ${port}`)
})

db.connect()
