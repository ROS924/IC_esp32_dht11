const Sequelize = require('sequelize')
const sequelize = new Sequelize('dht11', 'root', ''/*respectiva senha*/, {
    host:"localhost",
    dialect:'mysql',
    logging:true
})

module.exports = {
    Sequelize: Sequelize,
    sequelize: sequelize
}