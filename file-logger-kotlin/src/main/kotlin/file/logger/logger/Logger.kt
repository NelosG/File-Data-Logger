package file.logger.logger

import org.apache.poi.xssf.usermodel.XSSFRow
import org.apache.poi.xssf.usermodel.XSSFSheet
import org.apache.poi.xssf.usermodel.XSSFWorkbook
import java.nio.file.Path
import java.nio.file.StandardOpenOption
import java.time.LocalDate
import java.time.LocalDateTime
import java.util.*
import kotlin.io.path.outputStream


class Logger(private val path: Path) : AutoCloseable {

    constructor(stringPath: String) : this(Path.of(stringPath))

    private val workbook = XSSFWorkbook()
    private var createHelper = workbook.creationHelper
    private val dateTimeCellStyle = run {
        val cellStyle = workbook.createCellStyle()
        cellStyle.dataFormat = createHelper.createDataFormat().getFormat("h:mm m/d/yy")
        cellStyle
    }

    private lateinit var curSheet: XSSFSheet
    private val indexMap: MutableMap<String, Int> = mutableMapOf()
    private var index: Int = 0

    fun setSheet(name: String, vararg header: String) {
        if (::curSheet.isInitialized) {
            indexMap[curSheet.sheetName] = index
        }
        curSheet = workbook.getSheet(name) ?: workbook.createSheet(name)
        val headerRow = curSheet.createRow(0)
        header.forEachIndexed { idx, value ->
            headerRow.createCell(idx).setCellValue(value)
        }
        index = indexMap[curSheet.sheetName] ?: 1
    }

    fun println(vararg content: Any) {
        val row: XSSFRow
        synchronized(this) {
            row = curSheet.createRow(index++)
        }
        content.forEachIndexed { idx, value ->
            val cell = row.createCell(idx)
            when (value) {
                is LocalDate -> {
                    cell.setCellValue(value)
                    cell.cellStyle = dateTimeCellStyle
                }

                is LocalDateTime -> {
                    cell.setCellValue(value)
                    cell.cellStyle = dateTimeCellStyle
                }

                is Date -> cell.setCellValue(value) //TODO
                is Calendar -> cell.setCellValue(value) //TODO
                is String -> cell.setCellValue(value)
                is Boolean -> cell.setCellValue(value)
                is Double -> cell.setCellValue(value)
                is Float -> cell.setCellValue(value.toDouble())
                is Long -> cell.setCellValue(value.toDouble())
                is Int -> cell.setCellValue(value.toDouble())
                else -> error("Usupported type")
            }
        }
    }

    fun autoSizeColumns(vararg ignored: Int) {
        (0 until curSheet.first().lastCellNum).forEach {
            if (!ignored.contains(it)) {
                curSheet.autoSizeColumn(it)
            }
        }
    }

    fun flush() {
        workbook.write(path.outputStream(StandardOpenOption.CREATE, StandardOpenOption.CREATE))
    }

    override fun close() {
        workbook.close()
    }
}